#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

//静的メンバ変数の実態
const std::string FBXLoader::baseDirectory = "Resources/";

const std::string FBXLoader::defaultTextureFileName = "white1x1.png";

FBXLoader* FBXLoader::GetInstance()
{
    static FBXLoader instance;
    return &instance;
}

void FBXLoader::Initialize(ID3D12Device* device)
{
    //再初期化チェック
    assert(fbxManager == nullptr);

    //引数からメンバ変数に代入
    this->device = device;

    //FBXマネージャの生成
    fbxManager = FbxManager::Create();

    //FBXマネージャの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);

    //FBXインポータの生成
    fbxImporter = FbxImporter::Create(fbxManager, "");

}

void FBXLoader::Finalize()
{
    //各種FBXインスタンスの破棄
    fbxImporter->Destroy();

    fbxManager->Destroy();

}

//void FBXLoader::LoadModelFronmFile(const string& modelName)
FBXModel* FBXLoader::LoadModelFronmFile(const string& modelName)
{
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";

    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";

    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    //モデル生成
    FBXModel* fbxModel = new FBXModel();
    fbxModel->name = modelName;

    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();

    //あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    fbxModel->nodes.reserve(nodeCount);

    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());

    //FBXシーン解放
    fbxScene->Destroy();

    //バッファ生成
    fbxModel->CreateBuffers(device);

    return fbxModel;

}

void FBXLoader::ParseNodeRecursive(FBXModel* fbxModel, FbxNode* fbxNode, Node* parent)
{
    //ノード名を取得
    //string name = fbxNode->GetName();

    //モデルにノードを追加
    fbxModel->nodes.emplace_back();
    Node& node = fbxModel->nodes.back();

    //ノード名を取得
    node.name = fbxNode->GetName();

    //FBXノードのローカル移動情報
    //回転
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    //スケール
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    //移動
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //形式変換して代入
     //回転
    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    //スケール
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    //移動
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

    //回転角をDegree(度)からラジアンに変換
    node.rotation.x = XMConvertToRadians(node.rotation.x);
    node.rotation.y = XMConvertToRadians(node.rotation.y);
    node.rotation.z = XMConvertToRadians(node.rotation.z);

    //スケール、回転、平行移動行列の計算
    Matrix4 matScaling, matRotation, matTranslation;

    matScaling = ScaleMatrix4(node.scaling);
    matRotation = RotationMatrix4(node.rotation);
    matTranslation = MoveMatrix4(node.translation);

    //ローカル変形行列の計算
    node.transform = Affin::matUnit();

    //ワールド行列にスケーリングを反映
    node.transform *= matScaling;
    //ワールド行列に回転を反映
    node.transform *= matRotation;
    //ワールド行列に平行移動を反映
    node.transform *= matTranslation;

    //グローバル変形行列の計算
    node.globalTransform = node.transform;

    if (parent)
    {
        node.parent = parent;

        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;

    }

    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            fbxModel->meshNode = &node;
            ParseMesh(fbxModel, fbxNode);
        }
    }

    //子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
    }

}

void FBXLoader::ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode)
{
    //ノードのメッシュを取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //頂点座標読み取り
    ParseMeshVertices(fbxModel, fbxMesh);

    //面を構成するデータの読み取り
    ParseMeshFaces(fbxModel, fbxMesh);

    //マテリアルの読み取り
    ParseMaterial(fbxModel, fbxNode);

}

void FBXLoader::ParseMeshVertices(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
    auto& vertices = fbxModel->vertices;

    //頂点座標データの数
    const int controlPointsCount = fbxMesh->GetControlPointsCount();

    //必要数だけ頂点データ配列を確保
    FBXModel::VertexPosNormalUv vert{};
    fbxModel->vertices.resize(controlPointsCount, vert);

    //FBXメッシュの頂点座標配列を取得
    FbxVector4* pCoord = fbxMesh->GetControlPoints();

    //FBXメッシュの全頂点座標をモデル内の配列にコピーする
    for (int i = 0; i < controlPointsCount; i++)
    {
        FBXModel::VertexPosNormalUv& vertex = vertices[i];

        //座標コピー
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];

    }

}

void FBXLoader::ParseMeshFaces(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
    auto& vertices = fbxModel->vertices;
    auto& indices = fbxModel->indices;

    //1ファイルに複数メッシュのモデルは非対応
    assert(indices.size() == 0);

    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();

    //UVデータの数
    const int textureUVCount = fbxMesh->GetTextureUVCount();

    //UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    //面ごとの情報読み取り
    for (int i = 0; i < polygonCount; i++)
    {
        //面を構成する頂点の数を取得(3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++)
        {

            //FBX頂点配列のインデックス
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            //頂点法線読込
            FBXModel::VertexPosNormalUv& vertex = vertices[index];
            FbxVector4 normal;

            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];

            }

            //テクスチャUV読込
            if (textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;

                //0番決め打ちで読込
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }

            }

            //インデックス配列に頂点インデックス追加
            //3頂点までなら
            if (j < 3)
            {
                //1点追加し、他の2点と三角形を構築する
                indices.push_back(index);

            }
            else
            {
                //3点追加し、四角形の0,1,2,3の内　2,3,0で三角形を構築する
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];

                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);

            }

        }

    }

}

void FBXLoader::ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();

    if (materialCount > 0)
    {
        //先頭マテリアルを取得
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

        //テクスチャを読み込んだかどうかを表すフラグ
        bool textureLoaded = false;

        if (material)
        {
            //FbxSurfaceLambertクラスかどうかを調べる
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;

                fbxModel->ambient.x = (float)ambient.Get()[0];
                fbxModel->ambient.y = (float)ambient.Get()[1];
                fbxModel->ambient.z = (float)ambient.Get()[2];

                //拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;

                fbxModel->diffuse.x = (float)diffuse.Get()[0];
                fbxModel->diffuse.y = (float)diffuse.Get()[1];
                fbxModel->diffuse.z = (float)diffuse.Get()[2];

            }

            //ディフューズテクスチャを取り出す
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();

                if (texture)
                {
                    const char* filepath = texture->GetFileName();

                    //ファイルパスからファイル名抽出
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);

                    //テクスチャ読込
                    LoadTexture(fbxModel, baseDirectory + fbxModel->name + "/" + name);
                    textureLoaded = true;

                }

            }

        }

        //テクスチャがない場合は白テクスチャを貼る
        if (!textureLoaded)
        {
            LoadTexture(fbxModel, baseDirectory + defaultTextureFileName);
        }

    }

}

void FBXLoader::LoadTexture(FBXModel* fbxModel, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    //WICテクスチャのロード
    TexMetadata& metadata = fbxModel->metadata;
    ScratchImage& scratchImg = fbxModel->scratchImg;

    //ユニコード文字列に変換
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);

    if (FAILED(result))
    {
        assert(0);
    }

}

std::string FBXLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;

    //区切り文字 '\\' が出てくる一番最後の部分を検索
    pos1 = path.rfind('\\');

    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    //区切り文字 '/' が出てくる一番最後の部分を検索
    pos1 = path.rfind('/');

    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;
}
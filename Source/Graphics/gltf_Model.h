//#pragma once
//
//#include "../tinygltf-release/tiny_gltf.h"
//#include <d3d11.h>
//#include <wrl.h>
//#include <directxmath.h>
//#include <unordered_map>
//#include <string>
//#include <vector>
//#include <map>
//
//// GLTF Model Class
//class gltf_model
//{
//public:
//    struct Node
//    {
//        std::string name;
//        int skin{ -1 };  // Index of the skin associated with this node
//        int mesh{ -1 };  // Index of the mesh associated with this node
//
//        Node* parent = nullptr;  // Pointer to the parent node
//        std::vector<int> children;  // Indices of child nodes
//
//        // Local transforms
//        DirectX::XMFLOAT4 rotation{ 0,0,0,1 };
//        DirectX::XMFLOAT3 scale{ 1,1,1 };
//        DirectX::XMFLOAT3 translation{ 0,0,0 };
//
//        // Global transformation matrix
//        DirectX::XMFLOAT4X4 globalTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
//    };
//    std::vector<Node> nodes;
//
//    struct BufferView
//    {
//        DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
//        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
//        size_t stride_in_bytes{ 0 };
//        size_t size_in_bytes{ 0 };
//        size_t count() const { return size_in_bytes / stride_in_bytes; }
//    };
//
//    struct Mesh
//    {
//        std::string name;
//        struct Primitive
//        {
//            int material{ -1 };
//            std::map<std::string, BufferView> vertexBufferViews;
//            BufferView indexBufferView;
//        };
//        std::vector<Primitive> primitives;
//    };
//    std::vector<Mesh> meshes;
//
//    std::string filename;
//
//    struct TextureInfo
//    {
//        int index = -1;
//        int texcoord = 0;
//    };
//
//    struct NormalTextureInfo
//    {
//        int index = -1;
//        int texcoord = 0;
//        float scale = 1.0f;
//    };
//
//    struct OcclusionTextureInfo
//    {
//        int index = -1;
//        int texcoord = 0;
//        float strength = 1.0f;
//    };
//
//    struct PBRMetallicRoughness
//    {
//        float baseColorFactor[4] = { 1, 1, 1, 1 };
//        TextureInfo baseColorTexture;
//        float metallicFactor = 1.0f;
//        float roughnessFactor = 1.0f;
//        TextureInfo metallicRoughnessTexture;
//    };
//
//    struct Material
//    {
//        std::string name;
//        struct ConstantBuffer
//        {
//            float emissiveFactor[3] = { 0, 0, 0 };
//            int alphaMode = 0;     // "OPAQUE": 0, "MASK": 1, "BLEND": 2
//            float alphaCutoff = 0.5f;
//            bool doubleSided = false;
//
//            PBRMetallicRoughness pbrMetallicRoughness;
//
//            NormalTextureInfo normalTexture;
//            OcclusionTextureInfo occlusionTexture;
//            TextureInfo emissiveTexture;
//        };
//        ConstantBuffer data;
//    };
//    std::vector<Material> materials;
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> materialResourceView;
//
//    struct Texture
//    {
//        std::string name;
//        int source{ -1 };
//    };
//    std::vector<Texture> textures;
//
//    struct Image
//    {
//        std::string name;
//        int width{ -1 };
//        int height{ -1 };
//        int component{ -1 };
//        int bits{ -1 };
//        int pixelType{ -1 };
//        int bufferView;
//        std::string mimeType;
//        std::string uri;
//        bool asIs{ false };
//    };
//    std::vector<Image> images;
//    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureResourceViews;
//
//    struct Skin
//    {
//        std::vector<DirectX::XMFLOAT4X4> inverseBindMatrices;
//        std::vector<int> joints;
//    };
//    std::vector<Skin> skins;
//
//    struct Animation
//    {
//        std::string name;
//
//        struct Channel
//        {
//            int sampler{ -1 };
//            int targetNode{ -1 };
//            std::string targetPath;
//        };
//        std::vector<Channel> channels;
//
//        struct Sampler
//        {
//            int input{ -1 };
//            int output{ -1 };
//            std::string interpolation;
//        };
//        std::vector<Sampler> samplers;
//
//        std::unordered_map<int, std::vector<float>> timelines;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> scales;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT4>> rotations;
//        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> translations;
//    };
//    std::vector<Animation> animations;
//
//    struct Scene
//    {
//        std::string name;
//        std::vector<int> nodes;  // Array of 'root' nodes
//    };
//    std::vector<Scene> scenes;
//
//    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
//    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
//
//    struct PrimitiveConstants
//    {
//        DirectX::XMFLOAT4X4 world;
//        int material{ -1 };
//        int hasTangent{ 0 };
//        int skin{ -1 };
//        int pad;
//    };
//    Microsoft::WRL::ComPtr<ID3D11Buffer> primitiveCBuffer;
//
//    static const size_t PRIMITIVE_MAX_JOINTS = 512;
//    struct PrimitiveJointConstants
//    {
//        DirectX::XMFLOAT4X4 matrices[PRIMITIVE_MAX_JOINTS];
//    };
//    Microsoft::WRL::ComPtr<ID3D11Buffer> primitiveJointCBuffer;
//
//    gltf_model(ID3D11Device* device, const std::string& filename);
//    virtual ~gltf_model() = default;
//
//    void fetchNodes(const tinygltf::Model& gltfModel);
//    void cumulateTransforms(std::vector<Node>& nodes);
//    void fetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void fetchMaterials(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void fetchTextures(ID3D11Device* device, const tinygltf::Model& gltfModel);
//    void fetchAnimations(const tinygltf::Model& gltfModel);
//};

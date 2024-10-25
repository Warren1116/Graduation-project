//#include "gltf_Model.h"
//
//bool null_load_image_data(tinygltf::Image*, const int, std::string*, std::string*,
//    int, int, const unsigned char*, int, void*)
//{
//    return true;
//}
//
//gltf_model::gltf_model(ID3D11Device* device, const std::string& filename)
//{
//    tinygltf::TinyGLTF tiny_gltf;
//    tiny_gltf.SetImageLoader(null_load_image_data, nullptr);
//
//    tinygltf::Model gltf_model;
//    std::string error, warning;
//    bool succeeded = (filename.find(".glb") != std::string::npos) ?
//        tiny_gltf.LoadBinaryFromFile(&gltf_model, &error, &warning, filename.c_str()) :
//        tiny_gltf.LoadASCIIFromFile(&gltf_model, &error, &warning, filename.c_str());
//
//
//    fetchNodes(gltf_model);
//    fetchMeshes(device, gltf_model);
//    fetchMaterials(device, gltf_model);
//    fetchTextures(device, gltf_model);
//    fetchAnimations(gltf_model);
//}
//
//void gltf_model::fetchNodes(const tinygltf::Model& gltfModel)
//{
//    for (const auto& gltf_node : gltfModel.nodes)
//    {
//        Node node;
//        node.name = gltf_node.name;
//
//        // Translation
//        if (gltf_node.translation.size() == 3) {
//            node.translation = { gltf_node.translation[0], gltf_node.translation[1], gltf_node.translation[2] };
//        }
//
//        // Rotation
//        if (gltf_node.rotation.size() == 4) {
//            node.rotation = { gltf_node.rotation[0], gltf_node.rotation[1], gltf_node.rotation[2], gltf_node.rotation[3] };
//        }
//
//        // Scale
//        if (gltf_node.scale.size() == 3) {
//            node.scale = { gltf_node.scale[0], gltf_node.scale[1], gltf_node.scale[2] };
//        }
//
//        // Mesh
//        if (gltf_node.mesh >= 0) {
//            node.mesh = gltf_node.mesh;
//        }
//
//        // Skin
//        if (gltf_node.skin >= 0) {
//            node.skin = gltf_node.skin;
//        }
//
//        nodes.push_back(node);
//    }
//    cumulateTransforms(nodes);
//}
//
//void gltf_model::cumulateTransforms(std::vector<Node>& nodes)
//{
//    for (size_t i = 0; i < nodes.size(); ++i)
//    {
//        Node& node = nodes[i];
//
//        // Compute local transform
//        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
//
//        // Assuming node.rotation is a quaternion
//        DirectX::XMVECTOR quatRotation = DirectX::XMVectorSet(node.rotation.x, node.rotation.y, node.rotation.z, node.rotation.w);
//        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(quatRotation);
//
//        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
//
//        // Order: scale -> rotate -> translate
//        DirectX::XMMATRIX localTransform = translation * rotation * scale;
//
//        // Apply parent's global transform if it exists
//        if (node.parent)
//        {
//            DirectX::XMMATRIX parentTransform = DirectX::XMLoadFloat4x4(&node.parent->globalTransform);
//            localTransform *= parentTransform; // Apply parent's transformation
//        }
//
//        // Store the resulting global transform
//        DirectX::XMStoreFloat4x4(&node.globalTransform, localTransform);
//    }
//}
//
//void gltf_model::fetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    for (const auto& gltf_mesh : gltfModel.meshes)
//    {
//        Mesh mesh;
//        mesh.name = gltf_mesh.name;
//
//        for (const auto& gltf_primitive : gltf_mesh.primitives)
//        {
//            Mesh::Primitive primitive;
//            primitive.material = gltf_primitive.material;
//
//            // Fetch vertex buffer views
//            for (const auto& attribute : gltf_primitive.attributes)
//            {
//                const std::string& attribName = attribute.first;
//                int accessorIndex = attribute.second;
//
//                const tinygltf::Accessor& accessor = gltfModel.accessors[accessorIndex];
//                const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
//
//                // Calculate the size of the buffer and create it
//                BufferView buffer;
//                buffer.stride_in_bytes = accessor.ByteStride(bufferView);
//                buffer.size_in_bytes = accessor.count * buffer.stride_in_bytes;
//
//                // Create the buffer
//                D3D11_BUFFER_DESC bufferDesc = {};
//                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//                bufferDesc.ByteWidth = buffer.size_in_bytes;
//                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//                D3D11_SUBRESOURCE_DATA initData = {};
//                initData.pSysMem = reinterpret_cast<const void*>(gltfModel.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset);
//                HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &buffer.buffer);
//                if (FAILED(hr))
//                {
//                    // Handle buffer creation failure
//                }
//
//                primitive.vertexBufferViews[attribName] = buffer;
//            }
//
//            // Fetch index buffer view
//            if (gltf_primitive.indices >= 0)
//            {
//                const tinygltf::Accessor& indexAccessor = gltfModel.accessors[gltf_primitive.indices];
//                const tinygltf::BufferView& indexBufferView = gltfModel.bufferViews[indexAccessor.bufferView];
//
//                BufferView indexBuffer;
//                indexBuffer.stride_in_bytes = indexAccessor.ByteStride(indexBufferView);
//                indexBuffer.size_in_bytes = indexAccessor.count * indexBuffer.stride_in_bytes;
//
//                // Create the index buffer
//                D3D11_BUFFER_DESC indexBufferDesc = {};
//                indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//                indexBufferDesc.ByteWidth = indexBuffer.size_in_bytes;
//                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//                D3D11_SUBRESOURCE_DATA indexInitData = {};
//                indexInitData.pSysMem = reinterpret_cast<const void*>(gltfModel.buffers[indexBufferView.buffer].data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset);
//                HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, &indexBuffer.buffer);
//                if (FAILED(hr))
//                {
//                    // Handle buffer creation failure
//                }
//
//                primitive.indexBufferView = indexBuffer;
//            }
//
//            mesh.primitives.push_back(primitive);
//        }
//
//        meshes.push_back(mesh);
//    }
//}
//
//void gltf_model::fetchMaterials(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    for (const auto& gltf_material : gltfModel.materials)
//    {
//        Material material;
//        material.name = gltf_material.name;
//
//        // Fill in material properties
//        material.data.alphaMode = (gltf_material.alphaMode == "BLEND") ? 2 : (gltf_material.alphaMode == "MASK") ? 1 : 0;
//        material.data.alphaCutoff = gltf_material.alphaCutoff;
//        material.data.doubleSided = gltf_material.doubleSided;
//
//        // Fill in PBR properties
//        if (gltf_material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
//            material.data.pbrMetallicRoughness.baseColorTexture.index = gltf_material.pbrMetallicRoughness.baseColorTexture.index;
//        }
//        if (gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
//            material.data.pbrMetallicRoughness.metallicRoughnessTexture.index = gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index;
//        }
//
//        materials.push_back(material);
//    }
//}
//
//void gltf_model::fetchTextures(ID3D11Device* device, const tinygltf::Model& gltfModel)
//{
//    for (const auto& gltf_texture : gltfModel.textures)
//    {
//        Texture texture;
//        texture.name = gltf_texture.name;
//        texture.source = gltf_texture.source;
//
//        textures.push_back(texture);
//    }
//}
//
//void gltf_model::fetchAnimations(const tinygltf::Model& gltfModel)
//{
//    for (const auto& gltf_animation : gltfModel.animations)
//    {
//        Animation animation;
//        animation.name = gltf_animation.name;
//
//        // Fetch animation channels
//        for (const auto& channel : gltf_animation.channels)
//        {
//            Animation::Channel animChannel;
//            animChannel.sampler = channel.sampler;
//            animChannel.targetNode = channel.target_node;
//            animChannel.targetPath = channel.target_path;
//
//            animation.channels.push_back(animChannel);
//        }
//
//        // Fetch samplers
//        for (const auto& sampler : gltf_animation.samplers)
//        {
//            Animation::Sampler animSampler;
//            animSampler.input = sampler.input;
//            animSampler.output = sampler.output;
//            animSampler.interpolation = sampler.interpolation;
//
//            animation.samplers.push_back(animSampler);
//        }
//
//        animations.push_back(animation);
//    }
//}

#include "geometric_primitive.h"
#include <cstdio>
#include "misc.h"
#include <memory>
#include "Shader.h"
#include <vector>

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void GeometricPrimitive::Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    constants data{ world, material_color };
    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    immediate_context->HSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    immediate_context->DSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    immediate_context->GSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    index_buffer->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}

void GeometricPrimitive::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

}

Geometric_cube::Geometric_cube(ID3D11Device* device) : GeometricPrimitive(device)
{
    vertex vertices[24]
    {
    };


    uint32_t indices[36]
    {
    };

    //top
    uint32_t face = 0;
    vertices[face * 4 + 0] = { {-0.5f,+0.5f,+0.5f},{0.0f,+1.0f,0.0f} };
    vertices[face * 4 + 1] = { {+0.5f,+0.5f,+0.5f},{0.0f,+1.0f,0.0f} };
    vertices[face * 4 + 2] = { {-0.5f,+0.5f,-0.5f},{0.0f,+1.0f,0.0f} };
    vertices[face * 4 + 3] = { {+0.5f,+0.5f,-0.5f},{0.0f,+1.0f,0.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;


    //bottom
    face += 1;
    vertices[face * 4 + 0] = { {-0.5f,-0.5f,+0.5f},{0.0f,-1.0f,0.0f} };
    vertices[face * 4 + 1] = { {+0.5f,-0.5f,+0.5f},{0.0f,-1.0f,0.0f} };
    vertices[face * 4 + 2] = { {-0.5f,-0.5f,-0.5f},{0.0f,-1.0f,0.0f} };
    vertices[face * 4 + 3] = { {+0.5f,-0.5f,-0.5f},{0.0f,-1.0f,0.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;

    //front
    face += 1;
    vertices[face * 4 + 0] = { {-0.5f,+0.5f,-0.5f},{0.0f,0.0f,-1.0f} };
    vertices[face * 4 + 1] = { {+0.5f,+0.5f,-0.5f},{0.0f,0.0f,-1.0f} };
    vertices[face * 4 + 2] = { {-0.5f,-0.5f,-0.5f},{0.0f,0.0f,-1.0f} };
    vertices[face * 4 + 3] = { {+0.5f,-0.5f,-0.5f},{0.0f,0.0f,-1.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;

    //back
    face += 1;
    vertices[face * 4 + 0] = { {-0.5f,+0.5f,+0.5f},{0.0f,0.0f,+1.0f} };
    vertices[face * 4 + 1] = { {+0.5f,+0.5f,+0.5f},{0.0f,0.0f,+1.0f} };
    vertices[face * 4 + 2] = { {-0.5f,-0.5f,+0.5f},{0.0f,0.0f,+1.0f} };
    vertices[face * 4 + 3] = { {+0.5f,-0.5f,+0.5f},{0.0f,0.0f,+1.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;


    //right
    face += 1;
    vertices[face * 4 + 0] = { {+0.5f,+0.5f,-0.5f},{+1.0f,0.0f,0.0f} };
    vertices[face * 4 + 1] = { {+0.5f,+0.5f,+0.5f},{+1.0f,0.0f,0.0f} };
    vertices[face * 4 + 2] = { {+0.5f,-0.5f,-0.5f},{+1.0f,0.0f,0.0f} };
    vertices[face * 4 + 3] = { {+0.5f,-0.5f,+0.5f},{+1.0f,0.0f,0.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;
    //left
    face += 1;
    vertices[face * 4 + 0] = { {-0.5f,+0.5f,-0.5f},{-1.0f,0.0f,0.0f} };
    vertices[face * 4 + 1] = { {-0.5f,+0.5f,+0.5f},{-1.0f,0.0f,0.0f} };
    vertices[face * 4 + 2] = { {-0.5f,-0.5f,-0.5f},{-1.0f,0.0f,0.0f} };
    vertices[face * 4 + 3] = { {-0.5f,-0.5f,+0.5f},{-1.0f,0.0f,0.0f} };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;

    create_com_buffers(device, vertices, 24, indices, 36);

}

Geometric_cylinder::Geometric_cylinder(ID3D11Device* device, int segment) : GeometricPrimitive(device)
{
    float radius = 1.0f;
    float height = 2.0f;
    int num = segment;

}

Geometric_sphere::Geometric_sphere(ID3D11Device* device, uint32_t slices, uint32_t stacks) : GeometricPrimitive(device)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

    float r{ 0.5f };

    vertex top_vertex{};
    top_vertex.position = { 0.0f, +r, 0.0f };
    top_vertex.normal = { 0.0f, +1.0f, 0.0f };

    vertex bottom_vertex{};
    bottom_vertex.position = { 0.0f, -r, 0.0f };
    bottom_vertex.normal = { 0.0f, -1.0f, 0.0f };

    vertices.emplace_back(top_vertex);

    float phi_step{ DirectX::XM_PI / stacks };
    float theta_step{ 2.0f * DirectX::XM_PI / slices };

    for (uint32_t i = 1; i <= stacks - 1; ++i)
    {
        float phi{ i * phi_step };

        for (uint32_t j = 0; j <= slices; ++j)
        {
            float theta{ j * theta_step };

            vertex v{};

            v.position.x = r * sinf(phi) * cosf(theta);
            v.position.y = r * cosf(phi);
            v.position.z = r * sinf(phi) * sinf(theta);

            DirectX::XMVECTOR p{ XMLoadFloat3(&v.position) };
            DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

            vertices.emplace_back(v);
        }
    }

    vertices.emplace_back(bottom_vertex);

    for (uint32_t i = 1; i <= slices; ++i)
    {
        indices.emplace_back(0);
        indices.emplace_back(i + 1);
        indices.emplace_back(i);
    }

    uint32_t base_index{ 1 };
    uint32_t ring_vertex_count{ slices + 1 };
    for (uint32_t i = 0; i < stacks - 2; ++i)
    {
        for (uint32_t j = 0; j < slices; ++j)
        {
            indices.emplace_back(base_index + i * ring_vertex_count + j);
            indices.emplace_back(base_index + i * ring_vertex_count + j + 1);
            indices.emplace_back(base_index + (i + 1) * ring_vertex_count + j);

            indices.emplace_back(base_index + (i + 1) * ring_vertex_count + j);
            indices.emplace_back(base_index + i * ring_vertex_count + j + 1);
            indices.emplace_back(base_index + (i + 1) * ring_vertex_count + j + 1);
        }
    }

    uint32_t south_pole_index{ static_cast<uint32_t>(vertices.size() - 1) };

    base_index = south_pole_index - ring_vertex_count;

    for (uint32_t i = 0; i < slices; ++i)
    {
        indices.emplace_back(south_pole_index);
        indices.emplace_back(base_index + i);
        indices.emplace_back(base_index + i + 1);
    }
    create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());


}

// 円柱描画
void GeometricPrimitive::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
    Cylinder cylinder;
    cylinder.position = position;
    cylinder.radius = radius;
    cylinder.height = height;
    cylinder.color = color;
    cylinders.emplace_back(cylinder);
}

// 円柱メッシュ作成
void GeometricPrimitive::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks)
{
    cylinderVertexCount = 2 * slices * (stacks + 1) + 2 * slices;
    std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinderVertexCount);

    DirectX::XMFLOAT3* p = vertices.get();

    float stackHeight = height / stacks;
    float radiusStep = (radius2 - radius1) / stacks;

    // vertices of ring
    float dTheta = DirectX::XM_2PI / slices;

    for (int i = 0; i < slices; ++i)
    {
        int n = (i + 1) % slices;

        float c1 = cosf(i * dTheta);
        float s1 = sinf(i * dTheta);

        float c2 = cosf(n * dTheta);
        float s2 = sinf(n * dTheta);

        for (int j = 0; j <= stacks; ++j)
        {
            float y = start + j * stackHeight;
            float r = radius1 + j * radiusStep;

            p->x = r * c1;
            p->y = y;
            p->z = r * s1;
            p++;

            p->x = r * c2;
            p->y = y;
            p->z = r * s2;
            p++;
        }

        p->x = radius1 * c1;
        p->y = start;
        p->z = radius1 * s1;
        p++;

        p->x = radius2 * c1;
        p->y = start + height;
        p->z = radius2 * s1;
        p++;
    }

    // 頂点バッファ
    {
        D3D11_BUFFER_DESC desc = {};
        D3D11_SUBRESOURCE_DATA subresourceData = {};

        desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cylinderVertexCount);
        desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        subresourceData.pSysMem = vertices.get();
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;

        HRESULT hr = device->CreateBuffer(&desc, &subresourceData, cylinderVertexBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }
}


#include "Graphics/Graphics.h"
#include "Light.h"

Light::Light( LightType lightType )
: lightType( lightType )
{
}

void Light::PushRenderContext(RenderContext& rc) const
{
	// 登録されている光源の情報を設定
	switch (lightType)
	{
		case	LightType::Directional:
		{
			rc.directionalLightData.direction.x = direction.x;
			rc.directionalLightData.direction.y = direction.y;
			rc.directionalLightData.direction.z = direction.z;
			rc.directionalLightData.direction.w = 0.0f;
			rc.directionalLightData.color = color;

			break;
		}
	}
}

void Light::DrawDebugGUI()
{
	static constexpr char* lightTypeName[] =
	{
		"Directional",
	};

	if (ImGui::TreeNode(lightTypeName[static_cast<int>(lightType)]))
	{
		switch (lightType)
		{
			case	LightType::Directional:
			{
				if (ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f))
				{
					float l = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
					direction.x /= l;
					direction.y /= l;
					direction.z /= l;
				}
				ImGui::ColorEdit3("color", &color.x);
				ImGui::DragFloat("intensity", &color.w, 0.1f, 0, FLT_MAX);
				break;
			}
			case LightType::Spot:
			{
				ImGui::DragFloat3("position", &position.x);
				ImGui::DragFloat3("direction", &direction.x, 0.01f);
				DirectX::XMStoreFloat3(&direction,
									DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)));
				ImGui::ColorEdit3("color", &color.x);
				ImGui::DragFloat("intensity", &color.w, 0.1f, 0, FLT_MAX);
				ImGui::DragFloat("range", &range, 0.1f, 0, FLT_MAX);
				ImGui::SliderFloat("innerCorn", &innerCorn, 0, 1.0f);
				ImGui::SliderFloat("outerCorn", &outerCorn, 0, 1.0f);
				break;
			}
		}
		ImGui::TreePop();
	}
}

// デバッグ描画
void Light::DrawDebugPrimitive()
{
	DebugRenderer*	debugRenderer	= Graphics::Instance().GetDebugRenderer();
	LineRenderer*	lineRenderer	= Graphics::Instance().GetLineRenderer();

	switch (lightType)
	{
		case LightType::Directional:
		{
			//	平行光源は表示しない。
			break;
		}
	}
}

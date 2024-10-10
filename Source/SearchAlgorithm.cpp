//#include "Graphics/Graphics.h"
//#include "SearchAlgorithm.h"
//
//SearchAlgorithm::SearchAlgorithm()
//{
//	for (int i = 0; i < MAX_WAY_POINT; ++i)
//	{
//		findRoot.emplace_back(-1);
//	}
//}
//SearchAlgorithm::~SearchAlgorithm()
//{
//	searchEdge.clear();
//}
//
//bool SearchAlgorithm::DFSearch(Stage* stage)
//{
//	//�G�b�W�^�f�[�^�̃X�^�b�N���쐬
//	std::stack<Edge*> stack;
//
//	// �ߋ��̒T���f�[�^���N���A
//	SearchClear(stage);
//
//	//�X�^�[�gID��originNode �S�[��ID��distnationNode�Ɏ��_�~�[�G�b�W���쐬
//	Edge* edge = debug_new Edge();
//
//	//�_�~�[�G�b�W�͐ڑ����Ɛڑ���͊J�n�n�_�ɂ��Ă����B
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//	//�O�����Ƃ��ă_�~�[�G�b�W�f�[�^���X�^�b�N�����
//	stack.push(edge);
//
//	//DFS�T�[�`�A���S���Y��
//	// �X�^�b�N�Ƀf�[�^��������胋�[�v
//	while (!stack.empty())
//	{
//		//�X�^�b�N�ɋl�܂ꂽ�G�b�W�f�[�^�����o���A
//		Edge* nowEdge = stack.top();
//
//		//�T�[�`����Edge�̋L�^(���s���ʂ̕\���p�j
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		//�X�^�b�N�̃f�[�^������炷�B
//		stack.pop();
//
//		//�����A����nowEdge��distnationNode���S�[����������true��return����B
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		//nowEdge�̐�̃m�[�h���擾����B
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint];//�i�ݐ�̃m�[�h
//
//		wayPoint->searchFg = true;
//		//nowEdge�̐�̃m�[�h�ɓo�^���Ă���W�{�̃G�b�W���T�[�`���郋�[�v
//		for (int i = 0; i < EdgeNo; i++) {
//
//			// �G�b�W�����o��
//			Edge* edge = wayPoint->edge[i];
//
//			// ���o�����G�b�W�̐ڑ���͗L����(�����̏ꍇ�A-1���i�[����Ă���)
//			if (edge->destinationPoint >= 0) {
//
//				//�i�ݐ�̃m�[�h
//				WayPoint* nextPoint = stage->wayPoint[edge->destinationPoint];
//
//				//���̃G�b�W�̐�͒T���ς݂Ȃ�stack���Ȃ�
//				if (!nextPoint->searchFg) {
//
//					// �������m�[�h��T�����݂ɂ���
//					// nextPoint->searchFg = true;
//
//					//stack�ɓo�^
//					stack.push(edge);
//
//					//�T�[�`����Edge�̋L�^
//					searchEdge.push_back(edge);
//
//				}
//
//			}
//
//		}
//		//nowEdge�̐�̃m�[�h�ɓo�^���Ă���W�{�̃G�b�W���T�[�`���郋�[�v�I���
//	}
//	delete edge;
//	return false;
//
//}
//
//bool SearchAlgorithm::BFSearch(Stage* stage)
//{
//	// �ߋ��̒T���f�[�^���N���A
//	SearchClear(stage);
//	// TODO 06_01 DFS���Q�l��BFS�����������Ȃ���
//
//	// �X�^�[�gID��originNode�@�S�[��ID��distantionNode�Ɏ��_�~�[�G�b�W���쐬
//	Edge* edge = debug_new Edge();
//
//	// �_�~�[�G�b�W�͐ڑ����ƗD���͊J�n�n�_�ɂ��Ă���
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//
//	// �O�����Ƃ��ă_�~�[�G�b�W�f�[�^���X�^�b�N�����
//	//std::stack<Edge*> stack;
//	std::queue<Edge*> queue;
//	queue.push(edge);
//
//	// BFS�T�[�`�A���S���Y��
//	while (!queue.empty())
//	{
//		// �X�^�b�N�ɋl�܂ꂽ�G�b�W�f�[�^�����o��
//		//Edge* nowEdge = stack.top();
//		Edge* nowEdge = queue.front();
//
//		// �T�[�`����Edge�̋L�^(���s���ʂ̕\���p)
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		// �X�^�b�N�̃f�[�^������炷
//		queue.pop();
//
//		// �����A����nowEdge��distnationNode���S�[����������true��return����
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		// nowEdge�̐�̃m�[�h���擾����
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint]; // �i�ݐ�̃m�[�h
//		wayPoint->searchFg = true;
//
//		// nowEdge�̐�̃m�[�h�ɓo�^���Ă���8�{�̃G�b�W���T�[�`���郋�[�v
//		for (int i = 0; i < EdgeNo; i++)
//		{
//			// �G�b�W�����o��
//			Edge* edge = wayPoint->edge[i];
//
//			// ���o�����G�b�W�̐ڑ���͗L����(�������̏ꍇ�A-1���i�[����Ă���)
//			if (edge->destinationPoint >= 0)
//			{
//				// �i�ݐ�̃m�[�h
//				WayPoint* nextPoint = stage->wayPoint[edge->destinationPoint];
//
//				// ���̃G�b�W�̐�͒T���ς݂Ȃ�stack���Ȃ�
//				if (!nextPoint->searchFg)
//				{
//					// �������m�[�h��T���ς݂ɂ���
//					nextPoint->searchFg = true;
//
//					// stack�ɓo�^
//					queue.push(edge);
//
//					// �T�[�`����Edge�̋L�^
//					searchEdge.push_back(edge);
//				}
//			}
//		}
//		// nowEdge�̐�̃m�[�h�ɂƂ낭���Ă���8�{�̃G�b�W���T�[�`���郋�[�v�I���
//	}
//
//	delete edge;
//	return false;
//}
//
//bool SearchAlgorithm::DijkstraSearch(Stage* stage)
//{
//	//�G�b�W�^�f�[�^�̃t�����e�B�A�c���[���쐬
//	std::vector<Edge*> frontier;
//
//	// �ߋ��̒T���f�[�^���N���A
//	SearchClear(stage);
//
//	//�X�^�[�gID��originNode �S�[��ID��distnationNode�Ɏ��_�~�[�G�b�W���쐬
//	Edge* edge = debug_new Edge();
//
//	//�_�~�[�G�b�W�͐ڑ����Ɛڑ���͊J�n�n�_�ɂ��Ă����B
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//	// �R�X�g0���w��(�܂�Ԃ��ŃX�^�[�g�n�_�ɖ߂�Ȃ��悤�ɂ��邽��)
//	stage->wayPoint[edge->originPoint]->costFromStart = 0.0f;
//	//�O�����Ƃ��ă_�~�[�G�b�W�f�[�^���Z�b�g
//	Edge* nowEdge = edge;
//
//	//TODO 07_01 DijkstraSseach�A���S���Y��
//	// �������猊�����ɂȂ��Ă��鏈�����l�����߂Ă����Ȃ���
//	while (true)
//	{
//		//�T�[�`����Edge�̋L�^(���s���ʂ̕\���p�j
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		//�����A����nowEdge��distnationNode���S�[����������true��return����B
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		//nowEdge�̐�̃m�[�h���擾����B
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint];//�i�ݐ�̃m�[�h
//
//		//nowEdge�̐�̃m�[�h�ɓo�^���Ă���W�{�̃G�b�W���T�[�`���郋�[�v
//		for (int i = 0; i < EdgeNo; i++) {
//
//			Edge* nextEdge = wayPoint->edge[i];
//
//			if (nextEdge->destinationPoint >= 0) {//���̃G�b�W�͗L���ł���B
//
//				WayPoint* nextPoint = stage->wayPoint[nextEdge->destinationPoint];//�i�ݐ�̃m�[�h
//
//				// TODO 07_02 �i�ݐ�̃m�[�h�܂ł̃R�X�g���v�Z
//
//				// TODO 07_03 �i�ݐ�̃R�X�g���܂��v�Z����Ă��Ȃ����A�V�����R�X�g�̕����Ⴏ��Γo�^
//
//			}
//		}
//
//		nowEdge = searchMinCostEdge(frontier, stage);
//
//		if (nowEdge == nullptr)
//		{
//			delete edge;
//			return false;
//		}
//	}
//
//	//����������Ȃ����false;
//	return false;
//}
//bool SearchAlgorithm::AStarSearch(Stage* stage)
//{
//	//TODO 08_01 DijkstraSseach�A���S���Y�������ǂ��AA*�A���S���Y�����쐬���Ȃ���
//	SearchClear(stage);
//
//	return false;
//}
//
//Edge* SearchAlgorithm::searchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage)
//{
//	Edge* answer = nullptr;//�����̓��ꕨ
//	int  answerNo = 0;
//	float minCost = FLT_MAX;//�ŏ��̃R�X�g��ێ����郏�[�N
//
//	for (int fnrNo = 0; fnrNo < frontier.size(); fnrNo++) {
//
//		Edge* edge = frontier.at(fnrNo);
//
//		// TODO 07_04 �R�X�g�v�Z
//		// totalCost�ɐڑ����́u�X�^�[�g�ʒu����̃R�X�g�v�icostFromStart�j�{�G�b�W���̂����R�X�g�icost�j������
//		// element�ɓ����Ă���G�b�W�̐ڑ���m�[�h�́u�X�^�[�g�ʒu����̃R�X�g�v���v�Z����B
//		float totalCost = 0.0f;
//
//		// TODO 07_05 �R�X�g���o��
//		//�ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v��frontCost�Ɏ��o��(�܂��o�^����Ă��Ȃ��Ȃ�O�ƂȂ�)
//		float frontCost = 0.0f;
//
//		// TODO 07_06 �R�X�g�̔�r
//		//frontCost��0(�܂��o�^����Ă��Ȃ�)���AfrontCost��菭�Ȃ��R�X�g���[�g(>=totalCost)��
//		//�������ꂽ�Ȃ�A�ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v�icostFromStart�j��totalCost�ɏ��������B
//		//frontCost��totalCost�ɍX�V�B
//
//
//		// TODO 07_07 �G�b�W�̋L��
//		//frontCost���A�����ׂĂ���G�b�W�inowEdge�j�̐ڑ���̃g�[�^���R�X�g�ȏ�̂��̒���
//		//��ԏ������ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v�����G�b�W�𓚂��ianswer�j�Ƃ��ċL���B
//
//	}
//
//	if (answer == nullptr)return 0;
//	searchEdge.push_back(answer);//�T�[�`����Edge�̋L�^(�D�F���C��)
//
//	frontier.erase(frontier.begin() + answerNo);//�����̃G�b�W�̓_�C�N�X�g���̃T�[�`��₩��O��
//
//	//����FNR�ɓo�^����̂͂��̖߂�l�ɓo�^����Ă���m�[�h�̃G�b�W�Q
//	return answer;
//
//
//}
//Edge* SearchAlgorithm::HeuristicSearchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage)
//{
//	// TODO 08_02 searchMinCostEdge�����ǂ��AA*�p�̃R�X�g�v�Z�ɏ��������Ȃ����B
//	// �ʓr�p�ӂ��Ă���heuristicCulc���\�b�h���g�p���Ă��ǂ��B
//	Edge* answer = nullptr;//�����̓��ꕨ
//	return answer;
//}
//float SearchAlgorithm::heuristicCulc(WayPoint* w1, WayPoint* w2)
//{
//	// TODO 08_03 2�_�Ԃ̋��������̂܂܃R�X�g�Ƃ��Čv�Z���郁�\�b�h�ł��B�B
//	// cost�̊�����ς��邱�ƂŎ��s���ʂ��قȂ邱�Ƃ��m�F���Ȃ����B
//	// �� return cost.x * 0.3�Ȃ�
//	DirectX::XMVECTOR start, end;
//	start = DirectX::XMLoadFloat3(&w1->position);
//	end = DirectX::XMLoadFloat3(&w2->position);
//	DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(end, start);
//	DirectX::XMVECTOR dist = DirectX::XMVector3Length(DirectX::XMVectorSubtract(end, start));
//	DirectX::XMFLOAT3 cost;
//	DirectX::XMStoreFloat3(&cost, dist);
//	return cost.x;
//}
//// ID3D11DeviceContext::DrawIndexed: Index buffer has not enough space!���o��̂ō̗p������
//void SearchAlgorithm::AddSerchArrow(Stage* stage)
//{
//	for (const auto edge : searchEdge)
//	{
//		if (edge->destinationPoint < 0) continue;
//		// �T�������G�b�W����ŕ\��
//		Arrow* arrow = new Arrow();
//		arrow->SetPosition(stage->wayPoint[edge->originPoint]->position);
//		arrow->height = edge->cost;
//		arrow->color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
//		if (arrow->height > 2.0f)
//		{
//			arrow->height /= 5.0f;
//		}
//		arrow->radius = 0.05f;
//
//		// originPoint����destinationPoint�̊p�x���v�Z
//		DirectX::XMVECTOR origin = DirectX::XMLoadFloat3(&stage->wayPoint[edge->originPoint]->position);
//		DirectX::XMVECTOR destination = DirectX::XMLoadFloat3(&stage->wayPoint[edge->destinationPoint]->position);
//		DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(destination, origin);
//		DirectX::XMFLOAT3 v, out;
//		DirectX::XMStoreFloat3(&v, vector);
//		float f = sqrtf(v.x * v.x + v.z * v.z);
//		out.x = 0.0f;
//		out.y = atan2f(v.z, -v.x);
//		out.z = atan2f(f, v.y);
//
//		arrow->SetAngle(out);
//		arrow->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
//		ArrowManager::Instance().AddArrow(arrow);
//	}
//}
//float SearchAlgorithm::AddAnswerArrow(Stage* stage)
//{
//	float totalCost = 0.0f;
//	// �S�[�����W����C���f�b�N�X�ԍ����擾�B�S�[������`�悷��
//	int startIndex = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	int startNo = stage->NearWayPointIndex(Goal::Instance().GetPosition());
//	int endNo = -1;
//	
//	while (endNo != startIndex) {
//		endNo = findRoot[startNo];
//		if (endNo == -1)break;
//
//		// arrow�ŕ`�悷��ꍇ
//		// �T�������G�b�W����ŕ\��
//		Arrow* arrow = new Arrow();
//		arrow->SetPosition(stage->wayPoint[endNo]->position);
//
//		DirectX::XMVECTOR start, end;
//		start = DirectX::XMLoadFloat3(&stage->wayPoint[endNo]->position);
//		end = DirectX::XMLoadFloat3(&stage->wayPoint[startNo]->position);
//		DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(end, start);
//		DirectX::XMVECTOR dist = DirectX::XMVector3Length(DirectX::XMVectorSubtract(end, start));
//		DirectX::XMFLOAT3 cost;
//		DirectX::XMStoreFloat3(&cost, dist);
//		arrow->height = cost.x;
//		arrow->radius = 0.05f;
//		arrow->color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
//		totalCost += cost.x;
//
//		// start����end�̊p�x���v�Z
//		DirectX::XMFLOAT3 v, out;
//		DirectX::XMStoreFloat3(&v, vector);
//		float f = sqrtf(v.x * v.x + v.z * v.z);
//		out.x = 0.0f;
//		out.y = atan2f(v.z, -v.x);
//		out.z = atan2f(f, v.y);
//
//		arrow->SetAngle(out);
//		arrow->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
//		ArrowManager::Instance().AddArrow(arrow);
//
//		startNo = endNo;
//	}
//	return totalCost;
//}
//void SearchAlgorithm::SearchRender(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, Stage* stage)
//{
//	LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();
//
//	// �T�[�`�����G�b�W��`��
//	for (const auto edge : searchEdge)
//	{
//		// �T�������G�b�W��`��
//		lineRenderer->AddVertex(DirectX::XMFLOAT3(stage->wayPoint[edge->originPoint]->position.x,
//			stage->wayPoint[edge->originPoint]->position.y + 2.0f,
//			stage->wayPoint[edge->originPoint]->position.z),
//			DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
//		lineRenderer->AddVertex(DirectX::XMFLOAT3(stage->wayPoint[edge->destinationPoint]->position.x,
//			stage->wayPoint[edge->destinationPoint]->position.y + 2.0f,
//			stage->wayPoint[edge->destinationPoint]->position.z),
//			DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
//	}
//
//	lineRenderer->Render(dc, view, projection);
//
//}
//void SearchAlgorithm::SearchClear(Stage* stage)
//{
//	ArrowManager::Instance().Clear();
//	// �T���G�b�W���N���A
//	searchEdge.clear();
//
//	//�T�[�`�ς݂�FG�����Z�b�g
//	for (int i = 0; i < MAX_WAY_POINT; i++) {
//		stage->wayPoint[i]->searchFg = false;
//		stage->wayPoint[i]->costFromStart = -1.0f;
//		findRoot[i] = -1;
//	}
//}
//
//void SearchAlgorithm::GoldenPathSpawn(Stage* stage, DirectX::XMFLOAT3 startPosition)
//{
//}
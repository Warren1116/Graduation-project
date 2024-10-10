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
//	//エッジ型データのスタックを作成
//	std::stack<Edge*> stack;
//
//	// 過去の探索データをクリア
//	SearchClear(stage);
//
//	//スタートIDをoriginNode ゴールIDをdistnationNodeに持つダミーエッジを作成
//	Edge* edge = debug_new Edge();
//
//	//ダミーエッジは接続元と接続先は開始地点にしておく。
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//	//前準備としてダミーエッジデータをスタックいれる
//	stack.push(edge);
//
//	//DFSサーチアルゴリズム
//	// スタックにデータがある限りループ
//	while (!stack.empty())
//	{
//		//スタックに詰まれたエッジデータを取り出し、
//		Edge* nowEdge = stack.top();
//
//		//サーチしたEdgeの記録(実行結果の表示用）
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		//スタックのデータを一つ減らす。
//		stack.pop();
//
//		//もし、次のnowEdgeのdistnationNodeがゴールだったらtrueでreturnする。
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		//nowEdgeの先のノードを取得する。
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint];//進み先のノード
//
//		wayPoint->searchFg = true;
//		//nowEdgeの先のノードに登録してある８本のエッジをサーチするループ
//		for (int i = 0; i < EdgeNo; i++) {
//
//			// エッジを取り出す
//			Edge* edge = wayPoint->edge[i];
//
//			// 取り出したエッジの接続先は有効か(無効の場合、-1が格納されている)
//			if (edge->destinationPoint >= 0) {
//
//				//進み先のノード
//				WayPoint* nextPoint = stage->wayPoint[edge->destinationPoint];
//
//				//そのエッジの先は探査済みならstackしない
//				if (!nextPoint->searchFg) {
//
//					// 見つけたノードを探査ずみにする
//					// nextPoint->searchFg = true;
//
//					//stackに登録
//					stack.push(edge);
//
//					//サーチしたEdgeの記録
//					searchEdge.push_back(edge);
//
//				}
//
//			}
//
//		}
//		//nowEdgeの先のノードに登録してある８本のエッジをサーチするループ終わり
//	}
//	delete edge;
//	return false;
//
//}
//
//bool SearchAlgorithm::BFSearch(Stage* stage)
//{
//	// 過去の探索データをクリア
//	SearchClear(stage);
//	// TODO 06_01 DFSを参考にBFSを完成させなさい
//
//	// スタートIDをoriginNode　ゴールIDをdistantionNodeに持つダミーエッジを作成
//	Edge* edge = debug_new Edge();
//
//	// ダミーエッジは接続元と優先先は開始地点にしておく
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//
//	// 前準備としてダミーエッジデータをスタック入れる
//	//std::stack<Edge*> stack;
//	std::queue<Edge*> queue;
//	queue.push(edge);
//
//	// BFSサーチアルゴリズム
//	while (!queue.empty())
//	{
//		// スタックに詰まれたエッジデータを取り出し
//		//Edge* nowEdge = stack.top();
//		Edge* nowEdge = queue.front();
//
//		// サーチしたEdgeの記録(実行結果の表示用)
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		// スタックのデータを一つ減らす
//		queue.pop();
//
//		// もし、次のnowEdgeのdistnationNodeがゴールだったらtrueでreturnする
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		// nowEdgeの先のノードを取得する
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint]; // 進み先のノード
//		wayPoint->searchFg = true;
//
//		// nowEdgeの先のノードに登録してある8本のエッジをサーチするループ
//		for (int i = 0; i < EdgeNo; i++)
//		{
//			// エッジを取り出す
//			Edge* edge = wayPoint->edge[i];
//
//			// 取り出したエッジの接続先は有効か(向こうの場合、-1が格納されている)
//			if (edge->destinationPoint >= 0)
//			{
//				// 進み先のノード
//				WayPoint* nextPoint = stage->wayPoint[edge->destinationPoint];
//
//				// そのエッジの先は探査済みならstackしない
//				if (!nextPoint->searchFg)
//				{
//					// 見つけたノードを探査済みにする
//					nextPoint->searchFg = true;
//
//					// stackに登録
//					queue.push(edge);
//
//					// サーチしたEdgeの記録
//					searchEdge.push_back(edge);
//				}
//			}
//		}
//		// nowEdgeの先のノードにとろくしてある8本のエッジをサーチするループ終わり
//	}
//
//	delete edge;
//	return false;
//}
//
//bool SearchAlgorithm::DijkstraSearch(Stage* stage)
//{
//	//エッジ型データのフロンティアツリーを作成
//	std::vector<Edge*> frontier;
//
//	// 過去の探索データをクリア
//	SearchClear(stage);
//
//	//スタートIDをoriginNode ゴールIDをdistnationNodeに持つダミーエッジを作成
//	Edge* edge = debug_new Edge();
//
//	//ダミーエッジは接続元と接続先は開始地点にしておく。
//	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	edge->originPoint = edge->destinationPoint;
//	// コスト0を指定(折り返しでスタート地点に戻らないようにするため)
//	stage->wayPoint[edge->originPoint]->costFromStart = 0.0f;
//	//前準備としてダミーエッジデータをセット
//	Edge* nowEdge = edge;
//
//	//TODO 07_01 DijkstraSseachアルゴリズム
//	// ここから穴抜けになっている処理を考え埋めていきなさい
//	while (true)
//	{
//		//サーチしたEdgeの記録(実行結果の表示用）
//		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;
//
//		//もし、次のnowEdgeのdistnationNodeがゴールだったらtrueでreturnする。
//		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
//		{
//			delete edge;
//			return true;
//		}
//
//		//nowEdgeの先のノードを取得する。
//		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint];//進み先のノード
//
//		//nowEdgeの先のノードに登録してある８本のエッジをサーチするループ
//		for (int i = 0; i < EdgeNo; i++) {
//
//			Edge* nextEdge = wayPoint->edge[i];
//
//			if (nextEdge->destinationPoint >= 0) {//そのエッジは有効である。
//
//				WayPoint* nextPoint = stage->wayPoint[nextEdge->destinationPoint];//進み先のノード
//
//				// TODO 07_02 進み先のノードまでのコストを計算
//
//				// TODO 07_03 進み先のコストがまだ計算されていないか、新しいコストの方が低ければ登録
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
//	//何も見つからなければfalse;
//	return false;
//}
//bool SearchAlgorithm::AStarSearch(Stage* stage)
//{
//	//TODO 08_01 DijkstraSseachアルゴリズムを改良し、A*アルゴリズムを作成しなさい
//	SearchClear(stage);
//
//	return false;
//}
//
//Edge* SearchAlgorithm::searchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage)
//{
//	Edge* answer = nullptr;//答えの入れ物
//	int  answerNo = 0;
//	float minCost = FLT_MAX;//最小のコストを保持するワーク
//
//	for (int fnrNo = 0; fnrNo < frontier.size(); fnrNo++) {
//
//		Edge* edge = frontier.at(fnrNo);
//
//		// TODO 07_04 コスト計算
//		// totalCostに接続元の「スタート位置からのコスト」（costFromStart）＋エッジ自体が持つコスト（cost）を入れる
//		// elementに入っているエッジの接続先ノードの「スタート位置からのコスト」を計算する。
//		float totalCost = 0.0f;
//
//		// TODO 07_05 コスト取り出し
//		//接続先の「スタート位置からのコスト」をfrontCostに取り出す(まだ登録されていないなら０となる)
//		float frontCost = 0.0f;
//
//		// TODO 07_06 コストの比較
//		//frontCostが0(まだ登録されていない)か、frontCostより少ないコストルート(>=totalCost)が
//		//発見されたなら、接続先の「スタート位置からのコスト」（costFromStart）をtotalCostに書き換え。
//		//frontCostもtotalCostに更新。
//
//
//		// TODO 07_07 エッジの記憶
//		//frontCostが、今調べているエッジ（nowEdge）の接続先のトータルコスト以上のもの中で
//		//一番小さい接続先の「スタート位置からのコスト」を持つエッジを答え（answer）として記憶。
//
//	}
//
//	if (answer == nullptr)return 0;
//	searchEdge.push_back(answer);//サーチしたEdgeの記録(灰色ライン)
//
//	frontier.erase(frontier.begin() + answerNo);//答えのエッジはダイクストラのサーチ候補から外す
//
//	//次にFNRに登録するのはこの戻り値に登録されているノードのエッジ群
//	return answer;
//
//
//}
//Edge* SearchAlgorithm::HeuristicSearchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage)
//{
//	// TODO 08_02 searchMinCostEdgeを改良し、A*用のコスト計算に書き換えなさい。
//	// 別途用意しているheuristicCulcメソッドを使用しても良い。
//	Edge* answer = nullptr;//答えの入れ物
//	return answer;
//}
//float SearchAlgorithm::heuristicCulc(WayPoint* w1, WayPoint* w2)
//{
//	// TODO 08_03 2点間の距離をそのままコストとして計算するメソッドです。。
//	// costの割合を変えることで実行結果が異なることを確認しなさい。
//	// 例 return cost.x * 0.3など
//	DirectX::XMVECTOR start, end;
//	start = DirectX::XMLoadFloat3(&w1->position);
//	end = DirectX::XMLoadFloat3(&w2->position);
//	DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(end, start);
//	DirectX::XMVECTOR dist = DirectX::XMVector3Length(DirectX::XMVectorSubtract(end, start));
//	DirectX::XMFLOAT3 cost;
//	DirectX::XMStoreFloat3(&cost, dist);
//	return cost.x;
//}
//// ID3D11DeviceContext::DrawIndexed: Index buffer has not enough space!が出るので採用見送り
//void SearchAlgorithm::AddSerchArrow(Stage* stage)
//{
//	for (const auto edge : searchEdge)
//	{
//		if (edge->destinationPoint < 0) continue;
//		// 探査したエッジを矢印で表示
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
//		// originPointからdestinationPointの角度を計算
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
//	// ゴール座標からインデックス番号を取得。ゴールから描画する
//	int startIndex = stage->NearWayPointIndex(Start::Instance().GetPosition());
//	int startNo = stage->NearWayPointIndex(Goal::Instance().GetPosition());
//	int endNo = -1;
//	
//	while (endNo != startIndex) {
//		endNo = findRoot[startNo];
//		if (endNo == -1)break;
//
//		// arrowで描画する場合
//		// 探査したエッジを矢印で表示
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
//		// startからendの角度を計算
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
//	// サーチしたエッジを描画
//	for (const auto edge : searchEdge)
//	{
//		// 探査したエッジを描画
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
//	// 探索エッジをクリア
//	searchEdge.clear();
//
//	//サーチ済みのFGをリセット
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
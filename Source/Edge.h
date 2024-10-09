#pragma once
#include <DirectXMath.h>

// 0  /*エッジの表示　12時の方向*/
// 1  /*エッジの表示　2.5時の方向*/
// 2  /*エッジの表示　３時の方向*/
// 3  /*エッジの表示　4.5時の方向*/
// 4  /*エッジの表示　6時の方向*/
// 5  /*エッジの表示　7.5時の方向*/
// 6  /*エッジの表示　9時の方向*/
// 7  /*エッジの表示　10.5時の方向*/

//       7   0   1
//        ＼ | ／
//       6―   ―2
//        ／ | ＼
//       5   4   3

class Edge
{
public:
	//コンストラクタ
	Edge() :cost(0), originPoint(-1), destinationPoint(-1) {}
	~Edge() {}
	int originPoint = -1;
	int destinationPoint = -1;
	float cost = 0;
};
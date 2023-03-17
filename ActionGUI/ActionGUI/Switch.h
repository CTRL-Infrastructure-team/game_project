#pragma once
#include"Action.h"
#include"Act.h"

namespace action {
	//Switchについて 
	//Start,Endの場合
	// point=trueなら、actionが開始したときに合図する
	// point=falseなら、actionが終了したときに合図する
	//Reqの場合
	// point=trueなら、actionが実行中なら合図する
	// point=falseなら、actionが実行中でないときに合図する
	struct Switch {
		Action* action;
		bool point;
	};
}

namespace act {
	//Switchについて
	// point=trueなら、actが開始してからt秒後に合図
	// point=falseなら、actが終了してからt秒後に合図
	struct Switch {
		int32 arrow_id;
		String actname;
		double t;
		bool point;
	};
}

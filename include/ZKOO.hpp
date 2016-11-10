
# include "../src/User/ZKOO/ZKOOMaker.h"
# include "../src/User/ZKOO/ZKOO.h"

// 旧式のZKOOを扱うためのマクロです。
#define zkoo User::ZKOOMaker::Geti( )

// スクリーンタッチと同じように扱えるようになった新しいZKOOのマクロです。
#define inputzkoo User::ZKOO::Geti( )
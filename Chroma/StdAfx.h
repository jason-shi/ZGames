#ifdef	_MSC_VER
#pragma	warning(disable: 4201)
#pragma	warning(disable: 4100)
#pragma	warning(disable: 4786)
#pragma	warning(disable: 4800)
#pragma	warning(disable: 4996)
#endif

#include <vector>
#include <list>
#include <iostream>
#include <hash_map>
#include <time.h>
#include <functional>

#include <WinLib.h>
#include <ZDesign.h>
#include <Z2coo.h>
#include <Z2mov.h>
#include <ZProt.h>
#include <ZText.h>
#include <ZTimer.h>

#include <Common.h>
#include <Table.h>
#include <Sample\Resource.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZOmyD.lib")
		#pragma comment(lib, "WinLibD.lib")
		#pragma comment(lib, "FreeTypeD.lib")
		#pragma comment(lib, "Z2cooD.lib")
		#pragma comment(lib, "Z2movD.lib")
		#pragma comment(lib, "ZDesignD.lib")
		#pragma comment(lib, "ZProtD.lib")
		#pragma comment(lib, "ZTextD.lib")
		#pragma comment(lib, "ZTimerD.lib")
	#else
		#pragma comment(lib, "ZOmy.lib")
		#pragma comment(lib, "WinLib.lib")
		#pragma comment(lib, "FreeType.lib")
		#pragma comment(lib, "Z2coo.lib")
		#pragma comment(lib, "Z2mov.lib")
		#pragma comment(lib, "ZDesign.lib")
		#pragma comment(lib, "ZProt.lib")
		#pragma comment(lib, "ZText.lib")
		#pragma comment(lib, "ZTimer.lib")
	#endif
#endif

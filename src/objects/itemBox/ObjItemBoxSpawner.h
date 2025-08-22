#pragma once

namespace revisited::objects {
	struct ObjItemBoxSpawner {
		enum class ItemType : unsigned char {
			ROCKET,
			TEN_RING,
			TWENTY_RING,
			FIFTY_RING,
			ULTRA,
			MAGNETIC,
			INFINITE_BOOST
		};

		ItemType type;
		bool isAir;
		bool canAirDashRocket;
		bool doBounce;

		static const hh::fnd::RflClass rflClass;
		static const hh::fnd::RflTypeInfo rflTypeInfo;
	};
}

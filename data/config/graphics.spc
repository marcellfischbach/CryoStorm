point_light {
	shadow_map {
		size 104,
		filter "PCF",
	}
},

directional_light {
	shadow_map {
		size 2048,
		near 1.0,
		far 100.0,
		cascades 0.06 0.24,
		filter "PCF",
	}
},


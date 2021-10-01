//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_SAGE_GENERATED_GLANCE_H
#define IDEA_SAGE_GENERATED_GLANCE_H


bool get_solution_000(
	const double b,
	const double p0,
	const double c0,
	const double a,
	const double c1,
	double *out_param
) {

	/*
	 * p0 == 1/2*(a*b - ((a - 1)*b*c1 - (a - 1)*c0)*sqrt(-p0^2 + 1) - sqrt(a^2*b^2 + 2*(a^2 - 2*a + 1)*b*c0*c1
	 * - (4*(a - 1)*b^2 - a^2 + 2*a - 1)*c0^2 + ((a^2 - 2*a + 1)*b^2 + 4*a^2 - 4*a)*c1^2 - ((a^2 - 2*a + 1)*b^2*c1^2
	 * - 2*(a^2 - 2*a + 1)*b*c0*c1 + (a^2 - 2*a + 1)*c0^2)*p0^2 - 2*((a^2 - a)*b*c0 + ((a^2 - a)*b^2 + 2*a^2 - 2*a)*c1)
	 * *sqrt(-p0^2 + 1)))/((a - 1)*b*c0 + (a - 1)*c1)
	 */
	const double var_000 = a + -1;
	const double var_001 = var_000 * b;
	const double var_002 = var_001 * c0;
	const double var_003 = a + -1;
	const double var_004 = var_003 * c1;
	const double var_005 = var_002 + var_004;
	const double var_006 = std::pow(var_005, -1);
	const double var_007 = a * b;
	const double var_008 = a + -1;
	const double var_009 = var_008 * b;
	const double var_010 = var_009 * c1;
	const double var_011 = a + -1;
	const double var_012 = var_011 * c0;
	const double var_013 = var_012 * -1;
	const double var_014 = var_010 + var_013;
	const double var_015 = p0 * p0;
	const double var_016 = var_015 * -1;
	const double var_017 = var_016 + 1;
	if (var_017 < 0) { return false; }
	const double var_018 = std::sqrt(var_017);
	const double var_019 = var_014 * var_018;
	const double var_020 = var_019 * -1;
	const double var_021 = var_007 + var_020;
	const double var_022 = a * a;
	const double var_023 = b * b;
	const double var_024 = var_022 * var_023;
	const double var_025 = a * a;
	const double var_026 = a * -2;
	const double var_027 = var_025 + var_026;
	const double var_028 = var_027 + 1;
	const double var_029 = var_028 * b;
	const double var_030 = var_029 * c0;
	const double var_031 = var_030 * c1;
	const double var_032 = var_031 * 2;
	const double var_033 = var_024 + var_032;
	const double var_034 = a + -1;
	const double var_035 = b * b;
	const double var_036 = var_034 * var_035;
	const double var_037 = var_036 * 4;
	const double var_038 = a * a;
	const double var_039 = var_038 * -1;
	const double var_040 = var_037 + var_039;
	const double var_041 = a * 2;
	const double var_042 = var_040 + var_041;
	const double var_043 = var_042 + -1;
	const double var_044 = c0 * c0;
	const double var_045 = var_043 * var_044;
	const double var_046 = var_045 * -1;
	const double var_047 = var_033 + var_046;
	const double var_048 = a * a;
	const double var_049 = a * -2;
	const double var_050 = var_048 + var_049;
	const double var_051 = var_050 + 1;
	const double var_052 = b * b;
	const double var_053 = var_051 * var_052;
	const double var_054 = a * a;
	const double var_055 = var_054 * 4;
	const double var_056 = var_053 + var_055;
	const double var_057 = a * -4;
	const double var_058 = var_056 + var_057;
	const double var_059 = c1 * c1;
	const double var_060 = var_058 * var_059;
	const double var_061 = var_047 + var_060;
	const double var_062 = a * a;
	const double var_063 = a * -2;
	const double var_064 = var_062 + var_063;
	const double var_065 = var_064 + 1;
	const double var_066 = b * b;
	const double var_067 = var_065 * var_066;
	const double var_068 = c1 * c1;
	const double var_069 = var_067 * var_068;
	const double var_070 = a * a;
	const double var_071 = a * -2;
	const double var_072 = var_070 + var_071;
	const double var_073 = var_072 + 1;
	const double var_074 = var_073 * b;
	const double var_075 = var_074 * c0;
	const double var_076 = var_075 * c1;
	const double var_077 = var_076 * -2;
	const double var_078 = var_069 + var_077;
	const double var_079 = a * a;
	const double var_080 = a * -2;
	const double var_081 = var_079 + var_080;
	const double var_082 = var_081 + 1;
	const double var_083 = c0 * c0;
	const double var_084 = var_082 * var_083;
	const double var_085 = var_078 + var_084;
	const double var_086 = p0 * p0;
	const double var_087 = var_085 * var_086;
	const double var_088 = var_087 * -1;
	const double var_089 = var_061 + var_088;
	const double var_090 = a * a;
	const double var_091 = a * -1;
	const double var_092 = var_090 + var_091;
	const double var_093 = var_092 * b;
	const double var_094 = var_093 * c0;
	const double var_095 = a * a;
	const double var_096 = a * -1;
	const double var_097 = var_095 + var_096;
	const double var_098 = b * b;
	const double var_099 = var_097 * var_098;
	const double var_100 = a * a;
	const double var_101 = var_100 * 2;
	const double var_102 = var_099 + var_101;
	const double var_103 = a * -2;
	const double var_104 = var_102 + var_103;
	const double var_105 = var_104 * c1;
	const double var_106 = var_094 + var_105;
	const double var_107 = p0 * p0;
	const double var_108 = var_107 * -1;
	const double var_109 = var_108 + 1;
	if (var_109 < 0) { return false; }
	const double var_110 = std::sqrt(var_109);
	const double var_111 = var_106 * var_110;
	const double var_112 = var_111 * -2;
	const double var_113 = var_089 + var_112;
	if (var_113 < 0) { return false; }
	const double var_114 = std::sqrt(var_113);
	const double var_115 = var_114 * -1;
	const double var_116 = var_021 + var_115;
	const double var_117 = var_006 * var_116;
	const double var_118 = var_117 * 1/2;
	*out_param = var_118;
	return true
}
}
[[nodiscard]] inline
bool get_solution_001(
	const double b,
	const double p0,
	const double c0,
	const double a,
	const double c1,
	double *out_param
) {
	const double var_000 = a + -1;
	const double var_001 = var_000 * b;
	const double var_002 = var_001 * c0;
	const double var_003 = a + -1;
	const double var_004 = var_003 * c1;
	const double var_005 = var_002 + var_004;
	if (std::abs(var_005) < TOLERANCE) { return false; }
	const double var_006 = 1.0 / var_005;
	const double var_007 = a * b;
	const double var_008 = a + -1;
	const double var_009 = var_008 * b;
	const double var_010 = var_009 * c1;
	const double var_011 = a + -1;
	const double var_012 = var_011 * c0;
	const double var_013 = var_012 * -1;
	const double var_014 = var_010 + var_013;
	const double var_015 = p0 * p0;
	const double var_016 = var_015 * -1;
	const double var_017 = var_016 + 1;
	if (var_017 < 0) { return false; }
	const double var_018 = std::sqrt(var_017);
	const double var_019 = var_014 * var_018;
	const double var_020 = var_019 * -1;
	const double var_021 = var_007 + var_020;
	const double var_022 = a * a;
	const double var_023 = b * b;
	const double var_024 = var_022 * var_023;
	const double var_025 = a * a;
	const double var_026 = a * -2;
	const double var_027 = var_025 + var_026;
	const double var_028 = var_027 + 1;
	const double var_029 = var_028 * b;
	const double var_030 = var_029 * c0;
	const double var_031 = var_030 * c1;
	const double var_032 = var_031 * 2;
	const double var_033 = var_024 + var_032;
	const double var_034 = a + -1;
	const double var_035 = b * b;
	const double var_036 = var_034 * var_035;
	const double var_037 = var_036 * 4;
	const double var_038 = a * a;
	const double var_039 = var_038 * -1;
	const double var_040 = var_037 + var_039;
	const double var_041 = a * 2;
	const double var_042 = var_040 + var_041;
	const double var_043 = var_042 + -1;
	const double var_044 = c0 * c0;
	const double var_045 = var_043 * var_044;
	const double var_046 = var_045 * -1;
	const double var_047 = var_033 + var_046;
	const double var_048 = a * a;
	const double var_049 = a * -2;
	const double var_050 = var_048 + var_049;
	const double var_051 = var_050 + 1;
	const double var_052 = b * b;
	const double var_053 = var_051 * var_052;
	const double var_054 = a * a;
	const double var_055 = var_054 * 4;
	const double var_056 = var_053 + var_055;
	const double var_057 = a * -4;
	const double var_058 = var_056 + var_057;
	const double var_059 = c1 * c1;
	const double var_060 = var_058 * var_059;
	const double var_061 = var_047 + var_060;
	const double var_062 = a * a;
	const double var_063 = a * -2;
	const double var_064 = var_062 + var_063;
	const double var_065 = var_064 + 1;
	const double var_066 = b * b;
	const double var_067 = var_065 * var_066;
	const double var_068 = c1 * c1;
	const double var_069 = var_067 * var_068;
	const double var_070 = a * a;
	const double var_071 = a * -2;
	const double var_072 = var_070 + var_071;
	const double var_073 = var_072 + 1;
	const double var_074 = var_073 * b;
	const double var_075 = var_074 * c0;
	const double var_076 = var_075 * c1;
	const double var_077 = var_076 * -2;
	const double var_078 = var_069 + var_077;
	const double var_079 = a * a;
	const double var_080 = a * -2;
	const double var_081 = var_079 + var_080;
	const double var_082 = var_081 + 1;
	const double var_083 = c0 * c0;
	const double var_084 = var_082 * var_083;
	const double var_085 = var_078 + var_084;
	const double var_086 = p0 * p0;
	const double var_087 = var_085 * var_086;
	const double var_088 = var_087 * -1;
	const double var_089 = var_061 + var_088;
	const double var_090 = a * a;
	const double var_091 = a * -1;
	const double var_092 = var_090 + var_091;
	const double var_093 = var_092 * b;
	const double var_094 = var_093 * c0;
	const double var_095 = a * a;
	const double var_096 = a * -1;
	const double var_097 = var_095 + var_096;
	const double var_098 = b * b;
	const double var_099 = var_097 * var_098;
	const double var_100 = a * a;
	const double var_101 = var_100 * 2;
	const double var_102 = var_099 + var_101;
	const double var_103 = a * -2;
	const double var_104 = var_102 + var_103;
	const double var_105 = var_104 * c1;
	const double var_106 = var_094 + var_105;
	const double var_107 = p0 * p0;
	const double var_108 = var_107 * -1;
	const double var_109 = var_108 + 1;
	if (var_109 < 0) { return false; }
	const double var_110 = std::sqrt(var_109);
	const double var_111 = var_106 * var_110;
	const double var_112 = var_111 * -2;
	const double var_113 = var_089 + var_112;
	if (var_113 < 0) { return false; }
	const double var_114 = std::sqrt(var_113);
	const double var_115 = var_021 + var_114;
	const double var_116 = var_006 * var_115;
	const double var_117 = var_116 * 1/2;
	*out_param = var_117;
	return true
}




#endif //IDEA_SAGE_GENERATED_GLANCE_H

#include <string.h>
#include "res.h"

jl_resource_info_t *g_jl_resources = NULL;


/* 完美哈希表参数. */
static int32_t  *G  = NULL;
static uint16_t *S1 = NULL;
static uint16_t *S2 = NULL;
static int NG = 0;
static int NS = 0;

#if 0 /* 已弃用 (仅用来展示结构). */
static const jl_resource_info_t g_jl_resources[JL_RESOURCE_COUNT] = {
    { 0, 0, 284864, 0, 0, 0, "Alibaba-PuHuiTi-Regular.ttf" },
    { 1, 284864, 250164, 0, 0, 0, "AlibabaPuHuiTi-3_0-65-Medium.otf" },
    { 2, 535028, 163448, 0, 0, 0, "Roboto-Bold.ttf" },
    { 3, 698476, 127488, 0, 0, 0, "Roboto-Medium.ttf" },
    { 4, 825964, 382304, 0, 0, 0, "Roboto-Regular.ttf" },
    { 5, 1208268, 725, 160, 81, 2, "ad_axisBg_dark.png" },
    { 6, 1208993, 50364, 854, 480, 1, "ad_bg00_dark.jpg" },
    { 7, 1259357, 64046, 854, 480, 1, "ad_bg_dark.jpg" },
    { 8, 1323403, 391, 16, 10, 2, "ad_icon_dark.png" },
    { 9, 1323794, 470464, 0, 0, 2, "ad_meterProgress_dark.bin" },
    { 10, 1794258, 1016, 84, 8, 2, "ad_pointer_dark.png" },
    { 11, 1795274, 50857, 854, 480, 1, "cp_bg_dark.jpg" },
    { 12, 1846131, 328699, 0, 0, 2, "cp_ring1_dark.bin" },
    { 13, 2174830, 328699, 0, 0, 2, "cp_ring1_dark_cn.bin" },
    { 14, 2503529, 95914, 854, 480, 1, "db_bg_dark.jpg" },
    { 15, 2599443, 1801, 16, 60, 2, "db_pointer.png" },
    { 16, 2601244, 143, 128, 6, 2, "db_profg00_dark.png" },
    { 17, 2601387, 180976, 0, 0, 2, "db_progressBar00_dark.bin" },
    { 18, 2782363, 187216, 0, 0, 2, "db_progressBar10_dark.bin" },
    { 19, 2969579, 498, 42, 42, 2, "dsIcon_dark.png" },
    { 20, 2970077, 627, 40, 40, 2, "EscArrow.png" },
    { 21, 2970704, 12887, 794, 64, 2, "focusSel.png" },
    { 22, 2983591, 86486, 854, 480, 1, "Gps_bg_dark.jpg" },
    { 23, 3070077, 634816, 0, 0, 2, "Gps_ring1CN_dark.bin" },
    { 24, 3704893, 634816, 0, 0, 2, "Gps_ring1EN_dark.bin" },
    { 25, 4339709, 31773, 200, 200, 2, "d_car_pitch.png" },
    { 26, 4371482, 27078, 200, 200, 2, "d_car_roll.png" },
    { 27, 4398560, 68218, 854, 480, 1, "d_gradient_bg.jpg" },
    { 28, 4466778, 695, 25, 25, 2, "d_gradientpoint.png" },
    { 29, 4467473, 914128, 0, 0, 2, "d_left_y.bin" },
    { 30, 5381601, 914128, 0, 0, 2, "d_right_y.bin" },
    { 31, 6295729, 13121, 854, 480, 1, "mm_bg_dark.jpg" },
    { 32, 6308850, 940, 40, 40, 2, "mm_iconAbout.png" },
    { 33, 6309790, 1235, 40, 40, 2, "mm_iconAccelerationT.png" },
    { 34, 6311025, 1384, 40, 40, 2, "mm_iconAlSound.png" },
    { 35, 6312409, 1233, 40, 40, 2, "mm_iconBrakeTest.png" },
    { 36, 6313642, 1472, 40, 40, 2, "mm_iconBrightAdjustm.png" },
    { 37, 6315114, 1015, 40, 40, 2, "mm_iconBtnSound.png" },
    { 38, 6316129, 788, 40, 40, 2, "mm_iconCapacity.png" },
    { 39, 6316917, 1194, 40, 40, 2, "mm_iconClearCodes.png" },
    { 40, 6318111, 992, 40, 40, 2, "mm_iconDataStream.png" },
    { 41, 6319103, 846, 40, 40, 2, "mm_iconECTSet.png" },
    { 42, 6319949, 1341, 40, 40, 2, "mm_iconFuelCaliRa.png" },
    { 43, 6321290, 1054, 40, 40, 2, "mm_iconInitFuel.png" },
    { 44, 6322344, 813, 40, 40, 2, "mm_iconInitMile.png" },
    { 45, 6323157, 1177, 40, 40, 2, "mm_iconIsStopStar.png" },
    { 46, 6324334, 1587, 40, 40, 2, "mm_iconLanguage.png" },
    { 47, 6325921, 1210, 40, 40, 2, "mm_iconMileCaliRa.png" },
    { 48, 6327131, 1253, 40, 40, 2, "mm_iconOtaUpdate.png" },
    { 49, 6328384, 1011, 40, 40, 2, "mm_iconReadCodes.png" },
    { 50, 6329395, 1263, 40, 40, 2, "mm_iconRestoreDefa.png" },
    { 51, 6330658, 1274, 40, 40, 2, "mm_iconRestoreDefaobd.png" },
    { 52, 6331932, 714, 40, 40, 2, "mm_iconRpmSet.png" },
    { 53, 6332646, 1291, 40, 40, 2, "mm_iconScreenRotatio.png" },
    { 54, 6333937, 1342, 40, 40, 2, "mm_iconStartVol.png" },
    { 55, 6335279, 612, 40, 40, 2, "mm_iconThemeSettings.png" },
    { 56, 6335891, 1562, 40, 40, 2, "mm_iconTimeZoneSele.png" },
    { 57, 6337453, 2452, 40, 40, 2, "mm_iconTire.png" },
    { 58, 6339905, 1270, 40, 40, 2, "mm_iconUnit.png" },
    { 59, 6341175, 913, 40, 40, 2, "mm_iconUpLog.png" },
    { 60, 6342088, 711, 40, 40, 2, "mm_iconVOASet.png" },
    { 61, 6342799, 1630, 40, 40, 2, "mm_iconVolCali.png" },
    { 62, 6344429, 875, 40, 40, 2, "mm_iconVolumeAdjustment.png" },
    { 63, 6345304, 1015, 40, 40, 2, "mm_iconVssSet.png" },
    { 64, 6346319, 1784, 40, 40, 2, "mm_iconWIFI.png" },
    { 65, 6348103, 1035, 48, 48, 2, "addicon_dark.png" },
    { 66, 6349138, 566, 60, 80, 2, "addsub_unsel_dark.png" },
    { 67, 6349704, 865, 40, 40, 2, "light_max_dark.png" },
    { 68, 6350569, 724, 40, 40, 2, "light_min_dark.png" },
    { 69, 6351293, 606, 50, 80, 2, "num_unsel_dark.png" },
    { 70, 6351899, 501616, 0, 0, 2, "popMsg_bg_dark.bin" },
    { 71, 6853515, 983, 460, 48, 2, "slider_back_dark.png" },
    { 72, 6854498, 13245, 452, 40, 2, "slider_indi_dark.png" },
    { 73, 6867743, 198, 4, 20, 2, "slider_knob_dark.png" },
    { 74, 6867941, 966, 48, 48, 2, "subicon_dark.png" },
    { 75, 6868907, 840, 40, 40, 2, "volume_max_dark.png" },
    { 76, 6869747, 860, 40, 40, 2, "volume_min_dark.png" },
    { 77, 6870607, 7032, 118, 38, 2, "waitcar.png" },
    { 78, 6877639, 33458, 854, 480, 1, "NG_bg00.jpg" },
    { 79, 6911097, 694, 24, 10, 2, "NG_icon00.png" },
    { 80, 6911791, 561, 24, 10, 2, "NG_icon00_Focus.png" },
    { 81, 6912352, 14411, 180, 50, 2, "dtcFocus_dark.png" },
    { 82, 6926763, 504928, 0, 0, 2, "horShowDown_dark.bin" },
    { 83, 7431691, 504928, 0, 0, 2, "horShowDown_focus_dark.bin" },
    { 84, 7936619, 3655, 628, 98, 2, "horShowUp_dark.png" },
    { 85, 7940274, 227896, 0, 0, 2, "verticalShow_dark.bin" },
    { 86, 8168170, 54959, 854, 480, 1, "road_bg_dark.jpg" },
    { 87, 8223129, 14447, 298, 267, 2, "roadLeft01_dark.png" },
    { 88, 8237576, 14553, 298, 267, 2, "roadRight01_dark.png" },
    { 89, 8252129, 2569068, 854, 480, 0, "shutdown.gif" },
    { 90, 10821197, 2346, 794, 64, 2, "dsSelect.png" },
    { 91, 10823543, 37678, 855, 480, 1, "SL_bg_dark.jpg" },
    { 92, 10861221, 115264, 0, 0, 2, "SL_Spin02_dark.bin" },
    { 93, 10976485, 34908, 854, 480, 1, "update_bg_dark.jpg" },
    { 94, 11011393, 2395, 620, 28, 2, "update_progress_bg_dark.png" },
    { 95, 11013788, 1819, 620, 28, 2, "update_progress_fg_dark.png" },
};

/* 完美哈希表参数. */
static const int32_t G[] = {0, 0, 101, 0, 1, 0, 0, 126, 0, 0, 137, 0, 114, 0, 0, 0, 48, 131, 78, 111, 0, 34, 0, 0, 16, 139, 73, 0, 132, 75, 6, 32, 0, 26, 0, 16, 29, 11, 84, 37, 107, 138, 125, 0, 0, 56, 37, 49, 3, 135, 81, 0, 0, 0, 0, 74, 131, 0, 0, 79, 98, 0, 25, 129, 79, 15, 61, 111, 0, 76, 143, 55, 53, 49, 0, 63, 24, 0, 0, 129, 111, 0, 0, 73, 76, 0, 112, 125, 126, 98, 115, 58, 0, 30, 0, 28, 0, 59, 135, 53, 13, 65, 71, 0, 0, 29, 108, 127, 40, 100, 69, 0, 83, 107, 101, 0, 0, 0, 137, 118, 19, 121, 94, 33, 0, 108, 139, 9, 0, 0, 0, 62, 0, 89, 31, 0, 30, 94, 88, 124, 46, 0, 66, 118, 61};
static const uint8_t S1[] = {92, 3, 19, 61, 44, 65, 17, 135, 56, 134, 87, 69, 56, 82, 59, 42, 115, 94, 130, 54, 73, 55, 81, 117, 77, 39, 38, 4, 57, 45, 45, 63};
static const uint8_t S2[] = {47, 76, 133, 61, 29, 29, 118, 113, 54, 15, 86, 128, 31, 58, 69, 82, 53, 37, 96, 31, 7, 4, 116, 134, 94, 52, 73, 109, 134, 60, 29, 71};
static const int NG = 145;
static const int NS = 32;
#endif

static int hash_f(const char* key, const uint16_t* salt)
{
    int sum = 0;
    int len = strlen(key);
    for (int i = 0; i < len; i++) {
        sum += salt[i] * (uint8_t)key[i];
    }
    return sum % NG;
}

int perfect_hash(const char* key)
{
    if (strlen(key) > NS) return -1;
    
    int idx1 = hash_f(key, S1);
    int idx2 = hash_f(key, S2);
    
    return (G[idx1] + G[idx2]) % NG;
}

int res_init(void)
{
    /* 目录表. */
    g_jl_resources = (jl_resource_info_t *)(RES_IMG_BASE + g_res_hdr->table_offset);

    /* 完美哈希表参数. */
    jl_res_hash_meta_t *hash_meta = (jl_res_hash_meta_t *)(RES_IMG_BASE + g_res_hdr->hash_offset);
    G  = (int32_t *)(RES_IMG_BASE + hash_meta->G_offset);
    S1 = (uint16_t *)(RES_IMG_BASE + hash_meta->S1_offset);
    S2 = (uint16_t *)(RES_IMG_BASE + hash_meta->S2_offset);
    NG = hash_meta->NG;
    NS = hash_meta->NS;
    
    return 1;
}

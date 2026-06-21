#include "res.h"
#include <string.h>

const jl_resource_info_t g_jl_resources[JL_RESOURCE_COUNT] = {
    { 0u, 0u, 284760u, 0, 0, 0u, "Alibaba-PuHuiTi-Regular.ttf" },
    { 1u, 284760u, 250072u, 0, 0, 0u, "AlibabaPuHuiTi-3_0-65-Medium.otf" },
    { 2u, 534832u, 163448u, 0, 0, 0u, "Roboto-Bold.ttf" },
    { 3u, 698280u, 127488u, 0, 0, 0u, "Roboto-Medium.ttf" },
    { 4u, 825768u, 382304u, 0, 0, 0u, "Roboto-Regular.ttf" },
    { 5u, 1208072u, 725u, 160, 81, 2u, "ad_axisBg_dark.png" },
    { 6u, 1208797u, 23968u, 854, 480, 1u, "ad_bg00_dark.jpg" },
    { 7u, 1232765u, 41941u, 854, 480, 1u, "ad_bg_dark.jpg" },
    { 8u, 1274706u, 391u, 16, 10, 2u, "ad_icon_dark.png" },
    { 9u, 1275097u, 470464u, 0, 0, 2u, "ad_meterBg_dark.bin" },
    { 10u, 1745561u, 470464u, 0, 0, 2u, "ad_meterProgress_dark.bin" },
    { 11u, 2216025u, 1016u, 84, 8, 2u, "ad_pointer_dark.png" },
    { 12u, 2217041u, 50857u, 854, 480, 1u, "cp_bg_dark.jpg" },
    { 13u, 2267898u, 328699u, 0, 0, 2u, "cp_ring1_dark.bin" },
    { 14u, 2596597u, 328699u, 0, 0, 2u, "cp_ring1_dark_cn.bin" },
    { 15u, 2925296u, 47190u, 854, 480, 1u, "db_bg_dark.jpg" },
    { 16u, 2972486u, 4651u, 854, 50, 2u, "db_bottom_dark.png" },
    { 17u, 2977137u, 3566u, 35, 58, 2u, "db_gpsArrow.png" },
    { 18u, 2980703u, 476u, 202, 13, 2u, "db_gpsIcon_dark.png" },
    { 19u, 2981179u, 895u, 32, 32, 2u, "db_icon01_dark.png" },
    { 20u, 2982074u, 591u, 32, 32, 2u, "db_icon02_dark.png" },
    { 21u, 2982665u, 338704u, 0, 0, 2u, "db_meter00_dark.bin" },
    { 22u, 3321369u, 338704u, 0, 0, 2u, "db_meter01_dark.bin" },
    { 23u, 3660073u, 338704u, 0, 0, 2u, "db_meter10_dark.bin" },
    { 24u, 3998777u, 338704u, 0, 0, 2u, "db_meter11_dark.bin" },
    { 25u, 4337481u, 1801u, 16, 60, 2u, "db_pointer.png" },
    { 26u, 4339282u, 149u, 128, 6, 2u, "db_probg00_dark.png" },
    { 27u, 4339431u, 143u, 128, 6, 2u, "db_profg00_dark.png" },
    { 28u, 4339574u, 180976u, 0, 0, 2u, "db_progressBar00_dark.bin" },
    { 29u, 4520550u, 187216u, 0, 0, 2u, "db_progressBar10_dark.bin" },
    { 30u, 4707766u, 31621u, 614, 67, 2u, "db_top_dark.png" },
    { 31u, 4739387u, 498u, 42, 42, 2u, "dsIcon_dark.png" },
    { 32u, 4739885u, 627u, 40, 40, 2u, "EscArrow.png" },
    { 33u, 4740512u, 12887u, 794, 64, 2u, "focusSel.png" },
    { 34u, 4753399u, 86486u, 854, 480, 1u, "Gps_bg_dark.jpg" },
    { 35u, 4839885u, 634816u, 0, 0, 2u, "Gps_ring1CN_dark.bin" },
    { 36u, 5474701u, 634816u, 0, 0, 2u, "Gps_ring1EN_dark.bin" },
    { 37u, 6109517u, 31773u, 200, 200, 2u, "d_car_pitch.png" },
    { 38u, 6141290u, 27078u, 200, 200, 2u, "d_car_roll.png" },
    { 39u, 6168368u, 68218u, 854, 480, 1u, "d_gradient_bg.jpg" },
    { 40u, 6236586u, 695u, 25, 25, 2u, "d_gradientpoint.png" },
    { 41u, 6237281u, 914128u, 0, 0, 2u, "d_left_y.bin" },
    { 42u, 7151409u, 914128u, 0, 0, 2u, "d_right_y.bin" },
    { 43u, 8065537u, 13121u, 854, 480, 1u, "mm_bg_dark.jpg" },
    { 44u, 8078658u, 940u, 40, 40, 2u, "mm_iconAbout.png" },
    { 45u, 8079598u, 1235u, 40, 40, 2u, "mm_iconAccelerationT.png" },
    { 46u, 8080833u, 1384u, 40, 40, 2u, "mm_iconAlSound.png" },
    { 47u, 8082217u, 1233u, 40, 40, 2u, "mm_iconBrakeTest.png" },
    { 48u, 8083450u, 1472u, 40, 40, 2u, "mm_iconBrightAdjustm.png" },
    { 49u, 8084922u, 1015u, 40, 40, 2u, "mm_iconBtnSound.png" },
    { 50u, 8085937u, 788u, 40, 40, 2u, "mm_iconCapacity.png" },
    { 51u, 8086725u, 1194u, 40, 40, 2u, "mm_iconClearCodes.png" },
    { 52u, 8087919u, 992u, 40, 40, 2u, "mm_iconDataStream.png" },
    { 53u, 8088911u, 846u, 40, 40, 2u, "mm_iconECTSet.png" },
    { 54u, 8089757u, 1341u, 40, 40, 2u, "mm_iconFuelCaliRa.png" },
    { 55u, 8091098u, 1054u, 40, 40, 2u, "mm_iconInitFuel.png" },
    { 56u, 8092152u, 813u, 40, 40, 2u, "mm_iconInitMile.png" },
    { 57u, 8092965u, 1177u, 40, 40, 2u, "mm_iconIsStopStar.png" },
    { 58u, 8094142u, 1587u, 40, 40, 2u, "mm_iconLanguage.png" },
    { 59u, 8095729u, 1210u, 40, 40, 2u, "mm_iconMileCaliRa.png" },
    { 60u, 8096939u, 1253u, 40, 40, 2u, "mm_iconOtaUpdate.png" },
    { 61u, 8098192u, 1011u, 40, 40, 2u, "mm_iconReadCodes.png" },
    { 62u, 8099203u, 1263u, 40, 40, 2u, "mm_iconRestoreDefa.png" },
    { 63u, 8100466u, 1274u, 40, 40, 2u, "mm_iconRestoreDefaobd.png" },
    { 64u, 8101740u, 714u, 40, 40, 2u, "mm_iconRpmSet.png" },
    { 65u, 8102454u, 1291u, 40, 40, 2u, "mm_iconScreenRotatio.png" },
    { 66u, 8103745u, 1342u, 40, 40, 2u, "mm_iconStartVol.png" },
    { 67u, 8105087u, 612u, 40, 40, 2u, "mm_iconThemeSettings.png" },
    { 68u, 8105699u, 1562u, 40, 40, 2u, "mm_iconTimeZoneSele.png" },
    { 69u, 8107261u, 2452u, 40, 40, 2u, "mm_iconTire.png" },
    { 70u, 8109713u, 1270u, 40, 40, 2u, "mm_iconUnit.png" },
    { 71u, 8110983u, 913u, 40, 40, 2u, "mm_iconUpLog.png" },
    { 72u, 8111896u, 711u, 40, 40, 2u, "mm_iconVOASet.png" },
    { 73u, 8112607u, 1630u, 40, 40, 2u, "mm_iconVolCali.png" },
    { 74u, 8114237u, 875u, 40, 40, 2u, "mm_iconVolumeAdjustment.png" },
    { 75u, 8115112u, 1015u, 40, 40, 2u, "mm_iconVssSet.png" },
    { 76u, 8116127u, 1784u, 40, 40, 2u, "mm_iconWIFI.png" },
    { 77u, 8117911u, 1035u, 48, 48, 2u, "addicon_dark.png" },
    { 78u, 8118946u, 566u, 60, 80, 2u, "addsub_unsel_dark.png" },
    { 79u, 8119512u, 865u, 40, 40, 2u, "light_max_dark.png" },
    { 80u, 8120377u, 724u, 40, 40, 2u, "light_min_dark.png" },
    { 81u, 8121101u, 606u, 50, 80, 2u, "num_unsel_dark.png" },
    { 82u, 8121707u, 501616u, 0, 0, 2u, "popMsg_bg_dark.bin" },
    { 83u, 8623323u, 983u, 460, 48, 2u, "slider_back_dark.png" },
    { 84u, 8624306u, 13245u, 452, 40, 2u, "slider_indi_dark.png" },
    { 85u, 8637551u, 198u, 4, 20, 2u, "slider_knob_dark.png" },
    { 86u, 8637749u, 966u, 48, 48, 2u, "subicon_dark.png" },
    { 87u, 8638715u, 840u, 40, 40, 2u, "volume_max_dark.png" },
    { 88u, 8639555u, 860u, 40, 40, 2u, "volume_min_dark.png" },
    { 89u, 8640415u, 7032u, 118, 38, 2u, "waitcar.png" },
    { 90u, 8647447u, 33458u, 854, 480, 1u, "NG_bg00.jpg" },
    { 91u, 8680905u, 31577u, 854, 480, 1u, "NG_bg01.jpg" },
    { 92u, 8712482u, 694u, 24, 10, 2u, "NG_icon00.png" },
    { 93u, 8713176u, 561u, 24, 10, 2u, "NG_icon00_Focus.png" },
    { 94u, 8713737u, 623u, 24, 10, 2u, "NG_icon01.png" },
    { 95u, 8714360u, 634u, 24, 10, 2u, "NG_icon01_Focus.png" },
    { 96u, 8714994u, 14411u, 180, 50, 2u, "dtcFocus_dark.png" },
    { 97u, 8729405u, 504928u, 0, 0, 2u, "horShowDown_dark.bin" },
    { 98u, 9234333u, 504928u, 0, 0, 2u, "horShowDown_focus_dark.bin" },
    { 99u, 9739261u, 3655u, 628, 98, 2u, "horShowUp_dark.png" },
    { 100u, 9742916u, 227896u, 0, 0, 2u, "verticalShow_dark.bin" },
    { 101u, 9970812u, 3398u, 35, 58, 2u, "road_Arrow.png" },
    { 102u, 9974210u, 18603u, 854, 480, 1u, "road_bg_dark.jpg" },
    { 103u, 9992813u, 532u, 20, 20, 2u, "road_LeftIcon_dark.png" },
    { 104u, 9993345u, 529u, 20, 20, 2u, "road_RightIcon_dark.png" },
    { 105u, 9993874u, 17624u, 853, 76, 2u, "road_top_dark.png" },
    { 106u, 10011498u, 343996u, 0, 0, 2u, "roadLeft00_dark.bin" },
    { 107u, 10355494u, 238714u, 0, 0, 2u, "roadLeft01_dark.bin" },
    { 108u, 10594208u, 343996u, 0, 0, 2u, "roadRight00_dark.bin" },
    { 109u, 10938204u, 238714u, 0, 0, 2u, "roadRight01_dark.bin" },
    { 110u, 11176918u, 2569068u, 854, 480, 0u, "shutdown.gif" },
    { 111u, 13745986u, 2346u, 794, 64, 2u, "dsSelect.png" },
    { 112u, 13748332u, 24206u, 855, 480, 1u, "SL_bg_dark.jpg" },
    { 113u, 13772538u, 3165u, 50, 50, 2u, "SL_Icon_dark.png" },
    { 114u, 13775703u, 270016u, 0, 0, 2u, "SL_Spin01_dark.bin" },
    { 115u, 14045719u, 115264u, 0, 0, 2u, "SL_Spin02_dark.bin" },
    { 116u, 14160983u, 34908u, 854, 480, 1u, "update_bg_dark.jpg" },
    { 117u, 14195891u, 2395u, 620, 28, 2u, "update_progress_bg_dark.png" },
    { 118u, 14198286u, 1819u, 620, 28, 2u, "update_progress_fg_dark.png" },
};

/* 完美哈希表参数. */
static const int32_t G[] = {0, 0, 0, 0, 0, 0, 0, 0, 103, 35, 4, 106, 92, 18, 41, 44, 9, 166, 3, 0, 55, 0, 13, 11, 53, 95, 64, 0, 46, 0, 0, 0, 0, 31, 0, 0, 163, 38, 169, 73, 0, 107, 68, 0, 95, 0, 173, 40, 0, 0, 93, 148, 0, 0, 70, 178, 0, 0, 5, 176, 175, 19, 8, 152, 100, 64, 0, 19, 0, 2, 108, 20, 78, 86, 0, 41, 0, 30, 51, 69, 0, 10, 0, 1, 0, 89, 0, 94, 0, 58, 132, 0, 0, 0, 0, 63, 159, 17, 69, 0, 180, 6, 64, 127, 0, 130, 0, 107, 20, 55, 149, 0, 174, 0, 85, 139, 6, 179, 0, 22, 4, 0, 29, 2, 166, 26, 60, 34, 63, 4, 98, 73, 56, 9, 22, 27, 144, 0, 0, 80, 159, 28, 0, 25, 44, 79, 9, 34, 7, 0, 144, 0, 0, 0, 0, 1, 46, 0, 23, 146, 44, 45, 51, 41, 87, 95, 127, 110, 0, 176, 0, 0, 75, 0, 44, 161, 92, 0, 57, 0, 0};
static const uint8_t S1[] = {50, 136, 53, 53, 133, 37, 78, 97, 54, 58, 90, 45, 156, 125, 139, 64, 163, 114, 114, 29, 117, 76, 47, 83, 93, 171, 141, 75, 52, 164, 71, 123};
static const uint8_t S2[] = {20, 29, 80, 65, 129, 34, 122, 106, 105, 130, 77, 96, 17, 142, 48, 74, 7, 151, 30, 170, 28, 147, 68, 167, 18, 101, 47, 104, 41, 61, 150, 43};
static const int NG = 181;
static const int NS = 32;

static int hash_f(const char* key, const uint8_t* salt)
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

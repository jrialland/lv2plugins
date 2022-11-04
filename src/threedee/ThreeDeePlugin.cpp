#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

#include "ThreeDeePlugin.hpp"
#include <cmath>

void ThreeDeePlugin::set_sample_rate(double rate)
{
    fConst0 = expf(0.0f - 5e+01f / fminf(1.92e+05f, fmaxf(1.0f, rate)));
    fConst1 = 1.0f - fConst0;
}

void ThreeDeePlugin::run(uint32_t nsamples)
{
    float *input0 = get_buffer<THREEDEE_CHANNEL_IN>();
    float *output0 = get_buffer<THREEDEE_CHANNEL_OUT_LEFT>();
    float *output1 = get_buffer<THREEDEE_CHANNEL_OUT_RIGHT>();

    float fSlow0 = fConst1 * get_port_value<THREEDEE_CONTROL_RADIUS>();
    float fSlow1 = fConst1 * get_port_value<THREEDEE_CONTROL_ANGLE>();

    int i0;
    for (i0 = 0; i0 < nsamples; i0 = i0 + 1)
    {
        fRec0[0] = fSlow0 + fConst0 * fRec0[1];
        float fTemp0 = (float)(fRec0[0] <= 1.0f) + powf(fRec0[0], 2.0) * (float)(fRec0[0] > 1.0f);
        float fTemp1 = (float)(fRec0[0] >= 1.0f) + fRec0[0] * (float)(fRec0[0] < 1.0f);
        float fTemp2 = 2.0794415f * fTemp1;
        float fTemp3 = fTemp2 + -1.9459101f;
        float fTemp4 = 7.488876f * fTemp3;
        float fTemp5 = 7.488876f * fTemp3 * (float)(fTemp4 > 0.0f) * (float)(fTemp4 <= 1.0f) + (float)(fTemp4 > 1.0f);
        fRec1[0] = fSlow1 + fConst0 * fRec1[1];
        float fTemp6 = 7.0f * fRec1[0];
        float fTemp7 = cosf(fTemp6) * fTemp5;
        float fTemp8 = sinf(fTemp6) * fTemp5;
        float fTemp9 = fTemp2 + -1.7917595f;
        float fTemp10 = 6.4871593f * fTemp9;
        float fTemp11 = 6.4871593f * fTemp9 * (float)(fTemp10 > 0.0f) * (float)(fTemp10 <= 1.0f) + (float)(fTemp10 > 1.0f);
        float fTemp12 = 6.0f * fRec1[0];
        float fTemp13 = cosf(fTemp12) * fTemp11;
        float fTemp14 = sinf(fTemp12) * fTemp11;
        float fTemp15 = fTemp2 + -1.609438f;
        float fTemp16 = 5.484815f * fTemp15;
        float fTemp17 = 5.484815f * fTemp15 * (float)(fTemp16 > 0.0f) * (float)(fTemp16 <= 1.0f) + (float)(fTemp16 > 1.0f);
        float fTemp18 = 5.0f * fRec1[0];
        float fTemp19 = cosf(fTemp18) * fTemp17;
        float fTemp20 = sinf(fTemp18) * fTemp17;
        float fTemp21 = fTemp2 + -1.3862944f;
        float fTemp22 = 4.48142f * fTemp21;
        float fTemp23 = 4.48142f * fTemp21 * (float)(fTemp22 > 0.0f) * (float)(fTemp22 <= 1.0f) + (float)(fTemp22 > 1.0f);
        float fTemp24 = 4.0f * fRec1[0];
        float fTemp25 = 0.013257576f * sinf(fTemp24) * fTemp23;
        float fTemp26 = fTemp2 + -1.0986123f;
        float fTemp27 = 3.4760594f * fTemp26;
        float fTemp28 = 3.4760594f * fTemp26 * (float)(fTemp27 > 0.0f) * (float)(fTemp27 <= 1.0f) + (float)(fTemp27 > 1.0f);
        float fTemp29 = 3.0f * fRec1[0];
        float fTemp30 = cosf(fTemp29) * fTemp28;
        float fTemp31 = sinf(fTemp29) * fTemp28;
        float fTemp32 = fTemp2 + -0.6931472f;
        float fTemp33 = 2.4663033f * fTemp32;
        float fTemp34 = 2.4663033f * fTemp32 * (float)(fTemp33 > 0.0f) * (float)(fTemp33 <= 1.0f) + (float)(fTemp33 > 1.0f);
        float fTemp35 = 2.0f * fRec1[0];
        float fTemp36 = cosf(fTemp35) * fTemp34;
        float fTemp37 = sinf(fTemp35) * fTemp34;
        float fTemp38 = 3.0f * fTemp1;
        float fTemp39 = 3.0f * fTemp1 * (float)(fTemp38 > 0.0f) * (float)(fTemp38 <= 1.0f) + (float)(fTemp38 > 1.0f);
        float fTemp40 = fTemp39 * cosf(fRec1[0]);
        float fTemp41 = sinf(fRec1[0]) * fTemp39;
        float fTemp42 = 0.0625f - 0.041856002f * fTemp41 + 0.10104933f * fTemp40 - 0.051559865f * fTemp37 + 0.051559873f * fTemp36 - 0.033683088f * fTemp31 + 0.013952045f * fTemp30 - fTemp25 - 0.0030620992f * fTemp20 - 0.001268367f * fTemp19 - 0.0011620856f * fTemp14 - 0.0011620794f * fTemp13 - 0.00095007336f * fTemp8 - 0.00229369f * fTemp7;
        float fTemp43 = 0.013257576f * cosf(fTemp24) * fTemp23;
        float fTemp44 = 0.072916664f * fTemp37;
        float fTemp45 = 0.0016434328f * fTemp14;
        float fTemp46 = 0.0024826708f * fTemp8;
        float fTemp47 = 0.0016434328f * fTemp13;
        float fTemp48 = 0.003314394f * fTemp20;
        float fTemp49 = 0.036458332f * fTemp31;
        float fTemp50 = 0.072916664f * fTemp36;
        float fTemp51 = 0.109375f * fTemp41;
        float fTemp52 = 0.041856002f * fTemp41 + 0.0625f + 0.10104933f * fTemp40 + 0.051559873f * fTemp37 + 0.05155987f * fTemp36 + 0.033683106f * fTemp31 + 0.013952002f * fTemp30 + fTemp25 + 0.0030621008f * fTemp20 - 0.0012683637f * fTemp19 + 0.0011620824f * fTemp14 - 0.0011620824f * fTemp13 + 0.00095007714f * fTemp8 - 0.0022936887f * fTemp7;
        float fTemp53 = 0.7071068f * (0.109375f * fTemp40 + 0.0625f + fTemp50 + 0.036458332f * fTemp30 + fTemp43 + 0.003314394f * fTemp19 + fTemp47 + 0.0024826708f * fTemp7);
        float fTemp54 = (float)(input0[i0]) * (2.0794415f * (1.0f - fTemp1) + 1.0f);
        output0[i0] = (float)(fTemp54 * (fTemp53 + 0.9807853f * fTemp52 + 0.9876883f * (0.077339806f * fTemp41 + 0.0625f + 0.0773398f * fTemp40 + fTemp44 + 0.025779935f * fTemp31 - 0.025779935f * fTemp30 - fTemp43 - 0.0023436302f * fTemp20 - 0.0023436307f * fTemp19 - fTemp45 - 0.0017555136f * fTemp8 + 0.001755513f * fTemp7) + 0.9238795f * (0.10104932f * fTemp41 + 0.0625f + 0.041856006f * fTemp40 + 0.05155987f * fTemp37 - 0.05155987f * fTemp36 - 0.013951999f * fTemp31 - 0.03368311f * fTemp30 - fTemp25 - 0.0012683637f * fTemp20 + 0.0030621006f * fTemp19 + 0.0011620824f * fTemp14 + 0.0011620824f * fTemp13 + 0.0022936894f * fTemp8 - 0.00095007534f * fTemp7) + 0.80901694f * (fTemp43 + (fTemp51 + 0.0625f - fTemp50 - fTemp49) + fTemp48 - fTemp47 - fTemp46) + 0.649448f * (fTemp25 + 0.10104933f * fTemp41 + 0.0625f - 0.041856f * fTemp40 - 0.05155986f * fTemp37 - 0.051559877f * fTemp36 - 0.013952001f * fTemp31 + 0.033683106f * fTemp30 - 0.0012683636f * fTemp20 - 0.0030621008f * fTemp19 - 0.0011620824f * fTemp14 + 0.0011620825f * fTemp13 + 0.0022936887f * fTemp8 + 0.0009500766f * fTemp7) + 0.4539904f * (fTemp45 + 0.077339806f * fTemp41 + 0.0625f - 0.077339806f * fTemp40 - fTemp44 + 0.025779935f * fTemp31 + 0.025779935f * fTemp30 - fTemp43 - 0.0023436304f * fTemp20 + 0.0023436307f * fTemp19 - 0.0017555108f * fTemp8 - 0.0017555158f * fTemp7) + 0.23344523f * (0.04185601f * fTemp41 + 0.0625f - 0.10104932f * fTemp40 - 0.051559877f * fTemp37 + 0.05155986f * fTemp36 + 0.033683117f * fTemp31 - 0.013951976f * fTemp30 - fTemp25 + 0.0030621008f * fTemp20 + 0.0012683632f * fTemp19 - 0.0011620808f * fTemp14 - 0.001162084f * fTemp13 + 0.00095007464f * fTemp8 + 0.0022936896f * fTemp7) + 0.19509032f * fTemp42) / fTemp0);
        output1[i0] = (float)(fTemp54 * (fTemp53 + 0.19509032f * fTemp52 + 0.23344538f * (fTemp25 + (0.0625f - 0.041856f * fTemp41 - 0.10104933f * fTemp40 + 0.05155987f * fTemp37 + 0.05155987f * fTemp36 - 0.033683106f * fTemp31 - 0.0139520075f * fTemp30) - 0.003062099f * fTemp20 + 0.0012683677f * fTemp19 + 0.0011620829f * fTemp14 - 0.001162082f * fTemp13 - 0.0009500773f * fTemp8 + 0.0022936885f * fTemp7) + 0.45399052f * (fTemp44 + (0.0625f - 0.0773398f * fTemp41 - 0.07733981f * fTemp40) - 0.025779933f * fTemp31 + 0.025779936f * fTemp30 - fTemp43 + 0.0023436302f * fTemp20 + 0.0023436307f * fTemp19 - fTemp45 + 0.0017555145f * fTemp8 - 0.0017555121f * fTemp7) + 0.6494481f * (0.0625f - 0.10104932f * fTemp41 - 0.04185601f * fTemp40 + 0.051559884f * fTemp37 - 0.051559854f * fTemp36 + 0.013951977f * fTemp31 + 0.033683117f * fTemp30 - fTemp25 + 0.001268362f * fTemp20 - 0.0030621013f * fTemp19 + 0.001162082f * fTemp14 + 0.0011620829f * fTemp13 - 0.002293688f * fTemp8 + 0.0009500786f * fTemp7) + 0.809017f * (fTemp46 + (fTemp43 + fTemp49 + (0.0625f - fTemp51 - fTemp50) - fTemp48 - fTemp47)) + 0.92387956f * (fTemp25 + (0.0625f - 0.10104933f * fTemp41 + 0.041855995f * fTemp40 - 0.05155986f * fTemp37 - 0.051559877f * fTemp36 + 0.013952007f * fTemp31 - 0.033683106f * fTemp30) + 0.0012683627f * fTemp20 + 0.003062101f * fTemp19 - 0.001162084f * fTemp14 + 0.0011620808f * fTemp13 - 0.0022936873f * fTemp8 - 0.0009500802f * fTemp7) + 0.98768836f * (fTemp45 + (0.0625f - 0.07733981f * fTemp41 + 0.07733979f * fTemp40 - fTemp44 - 0.025779897f * fTemp31 - 0.025779972f * fTemp30 - fTemp43 + 0.002343632f * fTemp20 - 0.0023436288f * fTemp19) + 0.0017555133f * fTemp8 + 0.0017555134f * fTemp7) + 0.9807853f * fTemp42) / fTemp0);
        fRec0[1] = fRec0[0];
        fRec1[1] = fRec1[0];
    }
}

EXPORT_AUDIO_PLUGIN("https://github.com/jrialland/lv2plugins/threedee",
                    ThreeDeePlugin);
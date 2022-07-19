 #include "app_anc_assist_thirdparty.h"

 ThirdpartyAssistConfig thirdparty_assist_cfg;

 int32_t thirdparty_anc_assist_create(int sample_rate, int ch_num, int frame_size, const ThirdpartyAssistConfig *cfg)
{
	return 0;
}

int32_t thirdparty_anc_assist_destroy(void)
{
	return 0;
}

int32_t thirdparty_anc_assist_set_cfg(ThirdpartyAssistConfig *cfg)
{ 
  return 0;
}

ThirdpartyAssistRes thirdparty_anc_assist_process(float *ff_mic, float *fb_mic, float *talk_mic, float *ref, uint32_t frame_len)
{
  ThirdpartyAssistRes res;
  res.en = 0;
  return res;
}


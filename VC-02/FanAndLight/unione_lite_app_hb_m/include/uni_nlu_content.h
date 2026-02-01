#ifndef INC_UNI_NLU_CONTENT_H_
#define INC_UNI_NLU_CONTENT_H_

typedef struct {
  uni_u32 key_word_hash_code; /* 存放识别词汇对应的hashcode */
  uni_u8  nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char    *hash_collision_orginal_str /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */;
} uni_nlu_content_mapping_t;

enum {
  eCMD_wakeup_uni,
  eCMD_exitUni,
  eCMD_TurnOn,
  eCMD_OnLight,
  eCMD_OffLight,
  eCMD_FanOn,
  eCMD_FanOff,
  eCMD_IncreaseFan,
  eCMD_DecreaseFan,
};

const char* const g_nlu_content_str[] = {
[eCMD_wakeup_uni] = "{\"asr\":\"hello\",\"cmd\":\"wakeup_uni\",\"pcm\":\"[103, 104, 105]\"}",
[eCMD_exitUni] = "{\"asr\":\"quit\",\"cmd\":\"exitUni\",\"pcm\":\"[106]\"}",
[eCMD_TurnOn] = "{\"asr\":\"please light up the lamp\",\"cmd\":\"TurnOn\",\"pcm\":\"[108]\"}",
[eCMD_OnLight] = "{\"asr\":\"turn on the light\",\"cmd\":\"OnLight\",\"pcm\":\"[109]\"}",
[eCMD_OffLight] = "{\"asr\":\"turn off the light\",\"cmd\":\"OffLight\",\"pcm\":\"[109]\"}",
[eCMD_FanOn] = "{\"asr\":\"turn on the fan\",\"cmd\":\"FanOn\",\"pcm\":\"[109]\"}",
[eCMD_FanOff] = "{\"asr\":\"turn off the fan\",\"cmd\":\"FanOff\",\"pcm\":\"[109]\"}",
[eCMD_IncreaseFan] = "{\"asr\":\"increase fan speed\",\"cmd\":\"IncreaseFan\",\"pcm\":\"[109]\"}",
[eCMD_DecreaseFan] = "{\"asr\":\"decrease fan speed\",\"cmd\":\"DecreaseFan\",\"pcm\":\"[109]\"}",
};

/*TODO perf sort by hashcode O(logN), now version O(N)*/
const uni_nlu_content_mapping_t g_nlu_content_mapping[] = {
  {99162322U/*hello*/, eCMD_wakeup_uni, NULL},
  {1777618861U/*hello etech*/, eCMD_wakeup_uni, NULL},
  {3482191U/*quit*/, eCMD_exitUni, NULL},
  {1560432972U/*please light up the lamp*/, eCMD_TurnOn, NULL},
  {2767439195U/*please light up*/, eCMD_TurnOn, NULL},
  {991900133U/*light up*/, eCMD_TurnOn, NULL},
  {3717175913U/*turn on the light*/, eCMD_TurnOn, NULL},
  {2472855460U/*i am back*/, eCMD_TurnOn, NULL},
  {3717175913U/*turn on the light*/, eCMD_OnLight, "turn on the light"},
  {279435475U/*turn off the light*/, eCMD_OffLight, NULL},
  {1608329510U/*turn on the fan*/, eCMD_FanOn, NULL},
  {4058380816U/*turn off the fan*/, eCMD_FanOff, NULL},
  {670366876U/*increase fan speed*/, eCMD_IncreaseFan, NULL},
  {2065875960U/*decrease fan speed*/, eCMD_DecreaseFan, NULL},
};

#endif

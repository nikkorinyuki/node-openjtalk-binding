#include "open_jtalk.hpp"
/* Sub headers */
#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"
#include <iostream>
#define MAXBUFLEN 1024

void Open_JTalk_initialize(Open_JTalk *open_jtalk)
{
  MeCab::Mecab_initialize(&open_jtalk->mecab);
  NJD_initialize(&open_jtalk->njd);
  JPCommon_initialize(&open_jtalk->jpcommon);
  HTS_Engine_initialize(&open_jtalk->engine);
}

void Open_JTalk_clear(Open_JTalk *open_jtalk)
{
  MeCab::Mecab_clear(&open_jtalk->mecab);
  NJD_clear(&open_jtalk->njd);
  JPCommon_clear(&open_jtalk->jpcommon);
  HTS_Engine_clear(&open_jtalk->engine);
}

int Open_JTalk_load(Open_JTalk *open_jtalk, void *voice_data, size_t length_of_voice_data, const MeCab::ViterbiOptions &viterbi_options)
{
  if (MeCab::Mecab_load(&open_jtalk->mecab, viterbi_options) != TRUE)
  {
    Open_JTalk_clear(open_jtalk);
    return 1;
  }
  if (HTS_Engine_load(&open_jtalk->engine, &voice_data, &length_of_voice_data, 1) != TRUE)
  {
    Open_JTalk_clear(open_jtalk);
    return 2;
  }
  if (strcmp(HTS_Engine_get_fullcontext_label_format(&open_jtalk->engine), "HTS_TTS_JPN") != 0)
  {
    Open_JTalk_clear(open_jtalk);
    return 3;
  }
  return 0;
}

void Open_JTalk_set_sampling_frequency(Open_JTalk *open_jtalk, size_t i)
{
  HTS_Engine_set_sampling_frequency(&open_jtalk->engine, i);
}

void Open_JTalk_set_fperiod(Open_JTalk *open_jtalk, size_t i)
{
  HTS_Engine_set_fperiod(&open_jtalk->engine, i);
}

void Open_JTalk_set_alpha(Open_JTalk *open_jtalk, double f)
{
  HTS_Engine_set_alpha(&open_jtalk->engine, f);
}

void Open_JTalk_set_beta(Open_JTalk *open_jtalk, double f)
{
  HTS_Engine_set_beta(&open_jtalk->engine, f);
}

void Open_JTalk_set_speed(Open_JTalk *open_jtalk, double f)
{
  HTS_Engine_set_speed(&open_jtalk->engine, f);
}

void Open_JTalk_add_half_tone(Open_JTalk *open_jtalk, double f)
{
  HTS_Engine_add_half_tone(&open_jtalk->engine, f);
}

void Open_JTalk_set_msd_threshold(Open_JTalk *open_jtalk, size_t i, double f)
{
  HTS_Engine_set_msd_threshold(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_gv_weight(Open_JTalk *open_jtalk, size_t i, double f)
{
  HTS_Engine_set_gv_weight(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_volume(Open_JTalk *open_jtalk, double f)
{
  HTS_Engine_set_volume(&open_jtalk->engine, f);
}

void Open_JTalk_set_audio_buff_size(Open_JTalk *open_jtalk, size_t i)
{
  HTS_Engine_set_audio_buff_size(&open_jtalk->engine, i);
}

std::string njd_node_get_string(NJDNode *node)
{
  return std::string(NJDNode_get_string(node));
}

std::string njd_node_get_pos(NJDNode *node)
{
  return std::string(NJDNode_get_pos(node));
}

std::string njd_node_get_pos_group1(NJDNode *node)
{
  return std::string(NJDNode_get_pos_group1(node));
}

std::string njd_node_get_pos_group2(NJDNode *node)
{
  return std::string(NJDNode_get_pos_group2(node));
}

std::string njd_node_get_pos_group3(NJDNode *node)
{
  return std::string(NJDNode_get_pos_group3(node));
}

std::string njd_node_get_ctype(NJDNode *node)
{
  return std::string(NJDNode_get_ctype(node));
}

std::string njd_node_get_cform(NJDNode *node)
{
  return std::string(NJDNode_get_cform(node));
}

std::string njd_node_get_orig(NJDNode *node)
{
  return std::string(NJDNode_get_orig(node));
}

std::string njd_node_get_read(NJDNode *node)
{
  return std::string(NJDNode_get_read(node));
}

std::string njd_node_get_pron(NJDNode *node)
{
  return std::string(NJDNode_get_pron(node));
}

int njd_node_get_acc(NJDNode *node)
{
  return NJDNode_get_acc(node);
}

int njd_node_get_mora_size(NJDNode *node)
{
  return NJDNode_get_mora_size(node);
}

std::string njd_node_get_chain_rule(NJDNode *node)
{
  return std::string(NJDNode_get_chain_rule(node));
}

int njd_node_get_chain_flag(NJDNode *node)
{
  return NJDNode_get_chain_flag(node);
}

int Open_JTalk_run_frontend(Open_JTalk *open_jtalk, const char *txt, std::vector<std::string> *features)
{
  char *buff = (char *)malloc(strlen(txt) * 4 + 1);

  text2mecab(buff, txt);
  MeCab::Mecab_analysis(&open_jtalk->mecab, buff);
  mecab2njd(&open_jtalk->njd, MeCab::Mecab_get_feature(&open_jtalk->mecab),
            MeCab::Mecab_get_size(&open_jtalk->mecab));
  njd_set_pronunciation(&open_jtalk->njd);
  njd_set_digit(&open_jtalk->njd);
  njd_set_accent_phrase(&open_jtalk->njd);
  njd_set_accent_type(&open_jtalk->njd);
  njd_set_unvoiced_vowel(&open_jtalk->njd);
  njd_set_long_vowel(&open_jtalk->njd);

  /*njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
  JPCommon_make_label(&open_jtalk->jpcommon);*/

  /*std::vector<labels> _features = njd2feature(&open_jtalk->njd);
  feature2njd(&open_jtalk->njd, _features);*/
  njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
  JPCommon_make_label(&open_jtalk->jpcommon);

  *features = make_label(open_jtalk);

  HTS_Engine_refresh(&open_jtalk->engine);
  JPCommon_refresh(&open_jtalk->jpcommon);

  NJD_refresh(&open_jtalk->njd);
  MeCab::Mecab_refresh(&open_jtalk->mecab);
  free(buff);

  return 1;
}

labels node2feature(NJDNode *node)
{
  return {
      njd_node_get_string(node),
      njd_node_get_pos(node),
      njd_node_get_pos_group1(node),
      njd_node_get_pos_group2(node),
      njd_node_get_pos_group3(node),
      njd_node_get_ctype(node),
      njd_node_get_cform(node),
      njd_node_get_orig(node),
      njd_node_get_read(node),
      njd_node_get_pron(node),
      njd_node_get_acc(node),
      njd_node_get_mora_size(node),
      njd_node_get_chain_rule(node),
      njd_node_get_chain_flag(node)};
}

std::vector<labels> njd2feature(NJD *njd)
{
  NJDNode *node = njd->head;
  std::vector<labels> features = {};
  while (node != NULL)
  {
    features.push_back(node2feature(node));
    node = node->next;
  }
  return features;
}

std::vector<std::string> make_label(Open_JTalk *open_jtalk)
{
  int label_size = JPCommon_get_label_size(&open_jtalk->jpcommon);
  char **label_feature = JPCommon_get_label_feature(&open_jtalk->jpcommon);

  std::vector<std::string> labels = {};
  for (size_t i = 0; i < label_size; i++)
  {
    labels.push_back(label_feature[i]);
  }

  /*JPCommon_refresh(&open_jtalk->jpcommon);
  NJD_refresh(&open_jtalk->njd);*/
  return labels;
}

void feature2njd(NJD *njd, std::vector<labels> features)
{
  NJDNode *node;

  for (size_t i = 0; i < features.size(); i++)
  {
    labels feature_node = features[i];

    node = new NJDNode();
    NJDNode_initialize(node);
    NJDNode_set_string(node, feature_node.string.c_str());
    NJDNode_set_pos(node, feature_node.pos.c_str());
    NJDNode_set_pos_group1(node, feature_node.pos_group1.c_str());
    NJDNode_set_pos_group2(node, feature_node.pos_group2.c_str());
    NJDNode_set_pos_group3(node, feature_node.pos_group3.c_str());
    NJDNode_set_ctype(node, feature_node.ctype.c_str());
    NJDNode_set_cform(node, feature_node.cform.c_str());
    NJDNode_set_orig(node, feature_node.orig.c_str());
    NJDNode_set_read(node, feature_node.read.c_str());
    NJDNode_set_pron(node, feature_node.pron.c_str());
    NJDNode_set_acc(node, feature_node.acc);
    NJDNode_set_mora_size(node, feature_node.mora_size);
    NJDNode_set_chain_rule(node, feature_node.chain_rule.c_str());
    NJDNode_set_chain_flag(node, feature_node.chain_flag);
    NJD_push_node(njd, node);
  }
}

int Open_JTalk_synthesis(Open_JTalk *open_jtalk, const char *txt, signed short **pcm, size_t *length_of_pcm)
{
  int result = 0;
  char *buff = (char *)malloc(strlen(txt) * 4 + 1);

  text2mecab(buff, txt);
  MeCab::Mecab_analysis(&open_jtalk->mecab, buff);
  mecab2njd(&open_jtalk->njd, MeCab::Mecab_get_feature(&open_jtalk->mecab),
            MeCab::Mecab_get_size(&open_jtalk->mecab));
  njd_set_pronunciation(&open_jtalk->njd);
  njd_set_digit(&open_jtalk->njd);
  njd_set_accent_phrase(&open_jtalk->njd);
  njd_set_accent_type(&open_jtalk->njd);
  njd_set_unvoiced_vowel(&open_jtalk->njd);
  njd_set_long_vowel(&open_jtalk->njd);
  njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
  JPCommon_make_label(&open_jtalk->jpcommon);
  if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2)
  {
    if (HTS_Engine_synthesize_from_strings(&open_jtalk->engine, JPCommon_get_label_feature(&open_jtalk->jpcommon),
                                           JPCommon_get_label_size(&open_jtalk->jpcommon)) == TRUE)
      result = 1;
    HTS_Engine_save_pcm(&open_jtalk->engine, pcm, length_of_pcm);
    HTS_Engine_refresh(&open_jtalk->engine);
  }
  JPCommon_refresh(&open_jtalk->jpcommon);
  NJD_refresh(&open_jtalk->njd);
  MeCab::Mecab_refresh(&open_jtalk->mecab);
  free(buff);

  return result;
}

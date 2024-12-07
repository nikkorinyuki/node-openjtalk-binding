#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <string>

/* Main headers */
#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

typedef struct _Open_JTalk
{
   MeCab::Mecab mecab;
   NJD njd;
   JPCommon jpcommon;
   HTS_Engine engine;
} Open_JTalk;

typedef struct labels
{
   std::string string;
   std::string pos;
   std::string pos_group1;
   std::string pos_group2;
   std::string pos_group3;
   std::string ctype;
   std::string cform;
   std::string orig;
   std::string read;
   std::string pron;
   int acc;
   int mora_size;
   std::string chain_rule;
   int chain_flag;
} labels;

void Open_JTalk_initialize(Open_JTalk *open_jtalk);
void Open_JTalk_clear(Open_JTalk *open_jtalk);
int Open_JTalk_load(Open_JTalk *open_jtalk, void *voice_data, size_t length_of_voice_data, const MeCab::ViterbiOptions &viterbi_options);
void Open_JTalk_set_sampling_frequency(Open_JTalk *open_jtalk, size_t i);
void Open_JTalk_set_fperiod(Open_JTalk *open_jtalk, size_t i);
void Open_JTalk_set_alpha(Open_JTalk *open_jtalk, double f);
void Open_JTalk_set_beta(Open_JTalk *open_jtalk, double f);
void Open_JTalk_set_speed(Open_JTalk *open_jtalk, double f);
void Open_JTalk_add_half_tone(Open_JTalk *open_jtalk, double f);
void Open_JTalk_set_msd_threshold(Open_JTalk *open_jtalk, size_t i, double f);
void Open_JTalk_set_gv_weight(Open_JTalk *open_jtalk, size_t i, double f);
void Open_JTalk_set_volume(Open_JTalk *open_jtalk, double f);
void Open_JTalk_set_audio_buff_size(Open_JTalk *open_jtalk, size_t i);
std::string njd_node_get_string(NJDNode *node);
std::string njd_node_get_pos(NJDNode *node);
std::string njd_node_get_pos_group1(NJDNode *node);
std::string njd_node_get_pos_group2(NJDNode *node);
std::string njd_node_get_pos_group3(NJDNode *node);
std::string njd_node_get_ctype(NJDNode *node);
std::string njd_node_get_cform(NJDNode *node);
std::string njd_node_get_orig(NJDNode *node);
std::string njd_node_get_read(NJDNode *node);
std::string njd_node_get_pron(NJDNode *node);
int njd_node_get_acc(NJDNode *node);
int njd_node_get_mora_size(NJDNode *node);
std::string njd_node_get_chain_rule(NJDNode *node);
int njd_node_get_chain_flag(NJDNode *node);

int Open_JTalk_run_frontend(Open_JTalk *open_jtalk, const char *txt, std::vector<std::string> *features);
labels node2feature(NJDNode *node);
std::vector<labels> njd2feature(NJD *njd);
std::vector<std::string> make_label(Open_JTalk *open_jtalk);
void feature2njd(NJD *njd, std::vector<labels> features);

int Open_JTalk_synthesis(Open_JTalk *open_jtalk, const char *txt, signed short **pcm, size_t *length_of_pcm);
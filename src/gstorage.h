/**
 *    ______      ___
 *   / ____/___  /   | _____________  __________
 *  / / __/ __ \/ /| |/ ___/ ___/ _ \/ ___/ ___/
 * / /_/ / /_/ / ___ / /__/ /__/  __(__  |__  )
 * \____/\____/_/  |_\___/\___/\___/____/____/
 *
 * The MIT License (MIT)
 * Copyright (c) 2009-2025 Gerardo Orellana <hello @ goaccess.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GSTORAGE_H_INCLUDED
#define GSTORAGE_H_INCLUDED

#include "commons.h"
#include "parser.h"

#define DB_PATH "/tmp"

#define GAMTRC_TOTAL 8
/* Enumerated App Metrics */
typedef enum GAMetric_ {
  MTRC_DATES,
  MTRC_SEQS,
  MTRC_CNT_OVERALL,
  MTRC_HOSTNAMES,
  MTRC_LAST_PARSE,
  MTRC_JSON_LOGFMT,
  MTRC_METH_PROTO,
  MTRC_DB_PROPS,
} GAMetric;

/* Enumerated Storage Metrics */
typedef enum GSMetricType_ {
  /* uint32_t key - uint32_t val */
  MTRC_TYPE_II32,
  /* uint32_t key - string val */
  MTRC_TYPE_IS32,
  /* uint32_t key - uint64_t val */
  MTRC_TYPE_IU64,
  /* string key   - uint32_t val */
  MTRC_TYPE_SI32,
  /* string key   - uint8_t val */
  MTRC_TYPE_SI08,
  /* uint32_t key - uint8_t val */
  MTRC_TYPE_II08,
  /* string key   - string val */
  MTRC_TYPE_SS32,
  /* uint32_t key - GSLList val */
  MTRC_TYPE_IGSL,
  /* string key   - uint64_t val */
  MTRC_TYPE_SU64,
  /* uint32_t key - GKHashStorage_ val */
  MTRC_TYPE_IGKH,
  /* uint64_t key - uint32_t val */
  MTRC_TYPE_U648,
  /* uint64_t key - GLastParse val */
  MTRC_TYPE_IGLP,
} GSMetricType;

typedef struct GKHashMetric_ {
  union {
    GSMetric storem;
    GAMetric dbm;
  } metric;
  GSMetricType type;
  void *(*alloc) (void);
  void (*des) (void *, uint8_t free_data);
  void (*del) (void *, uint8_t free_data);
  uint8_t free_data:1;
  void *hash;
  const char *filename;
} GKHashMetric;

/* Each record contains a data value, i.e., Windows XP, and it may contain a
 * root value, i.e., Windows, and a unique key which is the combination of
 * date, IP and user agent */
typedef struct GKeyData_ {
  const void *data;
  uint32_t dhash;
  uint32_t data_nkey;
  uint32_t cdnkey;              /* cache data nkey */

  uint32_t rhash;
  const void *root;
  const void *root_key;
  uint32_t root_nkey;
  uint32_t crnkey;              /* cache root nkey */

  void *uniq_key;
  uint32_t uniq_nkey;

  uint32_t numdate;
} GKeyData;

typedef struct GParse_ {
  GModule module;
  int (*key_data) (GKeyData * kdata, GLogItem * logitem);

  /* data field */
  void (*datamap) (GModule module, GKeyData * kdata);
  void (*rootmap) (GModule module, GKeyData * kdata);
  void (*hits) (GModule module, GKeyData * kdata);
  void (*visitor) (GModule module, GKeyData * kdata);
  void (*bw) (GModule module, GKeyData * kdata, uint64_t size);
  void (*cumts) (GModule module, GKeyData * kdata, uint64_t ts);
  void (*maxts) (GModule module, GKeyData * kdata, uint64_t ts);
  void (*method) (GModule module, GKeyData * kdata, const char *data);
  void (*protocol) (GModule module, GKeyData * kdata, const char *data);
  void (*agent) (GModule module, GKeyData * kdata, uint32_t agent_nkey);
} GParse;

typedef struct httpmethods_ {
  const char *method;
  int len;
} httpmethods;

typedef struct httpprotocols_ {
  const char *protocol;
  int len;
} httpprotocols;

extern const httpmethods http_methods[];
extern const httpprotocols http_protocols[];
extern const size_t http_methods_len;
extern const size_t http_protocols_len;

const char *get_mtr_str (GSMetric metric);
int excluded_ip (GLogItem * logitem);
uint32_t *i322ptr (uint32_t val);
uint64_t *uint642ptr (uint64_t val);
void count_process_and_invalid (GLog * glog, GLogItem * logitem, const char *line);
void count_process (GLog * glog);
void free_gmetrics (GMetrics * metric);
void insert_methods_protocols (void);
void process_log (GLogItem * logitem);
void set_browser_os (GLogItem * logitem);
void set_data_metrics (GMetrics * ometrics, GMetrics ** nmetrics, GPercTotals totals);
void set_module_totals (GPercTotals * totals);
void uncount_invalid (GLog * glog);
void uncount_processed (GLog * glog);
GMetrics *new_gmetrics (void);

#endif // for #ifndef GSTORAGE_H

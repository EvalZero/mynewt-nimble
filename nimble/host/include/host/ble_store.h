/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BLE_STORE_
#define H_BLE_STORE_

#include <inttypes.h>

/* XXX: Rethink our/peer distinction.  Might make sense to unify keys such that
 * each entry contains both peers' addresses.
 */
#define BLE_STORE_OBJ_TYPE_MST_LTK      1
#define BLE_STORE_OBJ_TYPE_SLV_LTK      2
#define BLE_STORE_OBJ_TYPE_CCCD         3

#define BLE_STORE_ADDR_TYPE_NONE        0xff
#define BLE_STORE_AUTHREQ_NONE          0xff

struct ble_store_key_ltk {
    /**
     * Key by peer identity address;
     * peer_addr_type=BLE_STORE_ADDR_TYPE_NONE means don't key off peer.
     */
    uint8_t peer_addr[6];
    uint8_t peer_addr_type;

    /** Key by ediv; ediv_present=0 means don't key off ediv. */
    uint16_t ediv;
    unsigned ediv_present:1;

    /** Key by rand_num; rand_num_present=0 means don't key off rand_num. */
    uint64_t rand_num;
    unsigned rand_num_present:1;
};

struct ble_store_value_ltk {
    uint8_t peer_addr[6];
    uint8_t peer_addr_type;
    uint16_t ediv;
    uint64_t rand_num;
    uint8_t key[16];

    unsigned authenticated:1;
    unsigned sc:1;

};

struct ble_store_key_cccd {
    /**
     * Key by peer identity address;
     * peer_addr_type=BLE_STORE_ADDR_TYPE_NONE means don't key off peer.
     */
    uint8_t peer_addr[6];
    uint8_t peer_addr_type;

    /**
     * Key by characteristic value handle;
     * chr_val_handle=0 means don't key off characteristic handle.
     */
    uint16_t chr_val_handle;

    /** Number of results to skip; 0 means retrieve the first match. */
    uint8_t idx;
};

struct ble_store_value_cccd {
    uint8_t peer_addr[6];
    uint8_t peer_addr_type;
    uint16_t chr_val_handle;
    uint16_t flags;
    unsigned value_changed:1;
};

union ble_store_key {
    struct ble_store_key_ltk ltk;
    struct ble_store_key_cccd cccd;
};

union ble_store_value {
    struct ble_store_value_ltk ltk;
    struct ble_store_value_cccd cccd;
};

typedef int ble_store_read_fn(int obj_type, union ble_store_key *key,
                              union ble_store_value *dst);

typedef int ble_store_write_fn(int obj_type, union ble_store_value *val);

typedef int ble_store_delete_fn(int obj_type, union ble_store_key *key);

int ble_store_read(int obj_type, union ble_store_key *key,
                   union ble_store_value *val);
int ble_store_write(int obj_type, union ble_store_value *val);
int ble_store_delete(int obj_type, union ble_store_key *key);

int ble_store_read_slv_ltk(struct ble_store_key_ltk *key_ltk,
                           struct ble_store_value_ltk *value_ltk);
int ble_store_write_slv_ltk(struct ble_store_value_ltk *value_ltk);
int ble_store_read_mst_ltk(struct ble_store_key_ltk *key_ltk,
                           struct ble_store_value_ltk *value_ltk);
int ble_store_write_mst_ltk(struct ble_store_value_ltk *value_ltk);

int ble_store_read_cccd(struct ble_store_key_cccd *key,
                        struct ble_store_value_cccd *out_value);
int ble_store_write_cccd(struct ble_store_value_cccd *value);
int ble_store_delete_cccd(struct ble_store_key_cccd *key);

void ble_store_key_from_value_ltk(struct ble_store_key_ltk *out_key,
                                  struct ble_store_value_ltk *value);
void ble_store_key_from_value_cccd(struct ble_store_key_cccd *out_key,
                                   struct ble_store_value_cccd *value);

#endif

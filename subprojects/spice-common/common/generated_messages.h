/*
  Copyright (C) 2013 Red Hat, Inc.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

typedef struct SpiceWaitForChannel {
    uint8_t channel_type;
    uint8_t channel_id;
    uint64_t message_serial;
} SpiceWaitForChannel;

typedef struct SpiceMsgcAckSync {
    uint32_t generation;
} SpiceMsgcAckSync;

typedef struct SpiceMsgEmpty {
    char dummy[0];
} SpiceMsgEmpty;

typedef struct SpiceMsgData {
    uint8_t data[0];
} SpiceMsgData;

typedef struct SpiceMsgMigrate {
    uint32_t flags;
} SpiceMsgMigrate;

typedef struct SpiceMsgSetAck {
    uint32_t generation;
    uint32_t window;
} SpiceMsgSetAck;

typedef struct SpiceMsgPing {
    uint32_t id;
    uint64_t timestamp;
    uint32_t data_len;
    uint8_t *data;
} SpiceMsgPing;

typedef struct SpiceMsgWaitForChannels {
    uint8_t wait_count;
    SpiceWaitForChannel wait_list[0];
} SpiceMsgWaitForChannels;

typedef struct SpiceMsgDisconnect {
    uint64_t time_stamp;
    uint32_t reason;
} SpiceMsgDisconnect;

typedef struct SpiceMsgNotify {
    uint64_t time_stamp;
    uint32_t severity;
    uint32_t visibilty;
    uint32_t what;
    uint32_t message_len;
    uint8_t message[0];
} SpiceMsgNotify;

typedef struct SpiceChannelId {
    uint8_t type;
    uint8_t id;
} SpiceChannelId;

typedef struct SpiceMigrationDstInfo {
    uint16_t port;
    uint16_t sport;
    uint32_t host_size;
    uint8_t *host_data;
    uint32_t cert_subject_size;
    uint8_t *cert_subject_data;
} SpiceMigrationDstInfo;

typedef struct SpiceMsgcClientInfo {
    uint64_t cache_size;
} SpiceMsgcClientInfo;

typedef struct SpiceMsgcMainMouseModeRequest {
    uint16_t mode;
} SpiceMsgcMainMouseModeRequest;

typedef struct SpiceMsgMainAgentTokens {
    uint32_t num_tokens;
} SpiceMsgMainAgentTokens;

typedef struct SpiceMsgcMainMigrateDstDoSeamless {
    uint32_t src_version;
} SpiceMsgcMainMigrateDstDoSeamless;

typedef struct SpiceMsgMainMigrationBegin {
    SpiceMigrationDstInfo dst_info;
} SpiceMsgMainMigrationBegin;

typedef struct SpiceMsgMainInit {
    uint32_t session_id;
    uint32_t display_channels_hint;
    uint32_t supported_mouse_modes;
    uint32_t current_mouse_mode;
    uint32_t agent_connected;
    uint32_t agent_tokens;
    uint32_t multi_media_time;
    uint32_t ram_hint;
} SpiceMsgMainInit;

typedef struct SpiceMsgChannels {
    uint32_t num_of_channels;
    SpiceChannelId channels[0];
} SpiceMsgChannels;

typedef struct SpiceMsgMainMouseMode {
    uint16_t supported_modes;
    uint16_t current_mode;
} SpiceMsgMainMouseMode;

typedef struct SpiceMsgMainMultiMediaTime {
    uint32_t time;
} SpiceMsgMainMultiMediaTime;

typedef struct SpiceMsgMainAgentDisconnect {
    uint32_t error_code;
} SpiceMsgMainAgentDisconnect;

typedef struct SpiceMsgMainMigrationSwitchHost {
    uint16_t port;
    uint16_t sport;
    uint32_t host_size;
    uint8_t *host_data;
    uint32_t cert_subject_size;
    uint8_t *cert_subject_data;
} SpiceMsgMainMigrationSwitchHost;

typedef struct SpiceMsgMainName {
    uint32_t name_len;
    uint8_t name[0];
} SpiceMsgMainName;

typedef struct SpiceMsgMainUuid {
    uint8_t uuid[16];
} SpiceMsgMainUuid;

typedef struct SpiceMsgMainAgentConnectedTokens {
    uint32_t num_tokens;
} SpiceMsgMainAgentConnectedTokens;

typedef struct SpiceMsgMainMigrateBeginSeamless {
    SpiceMigrationDstInfo dst_info;
    uint32_t src_mig_version;
} SpiceMsgMainMigrateBeginSeamless;

typedef struct SpiceMsgDisplayBase {
    uint32_t surface_id;
    SpiceRect box;
    SpiceClip clip;
} SpiceMsgDisplayBase;

typedef struct SpiceResourceID {
    uint8_t type;
    uint64_t id;
} SpiceResourceID;

typedef struct SpiceStreamDataHeader {
    uint32_t id;
    uint32_t multi_media_time;
} SpiceStreamDataHeader;

typedef struct SpiceHead {
    uint32_t monitor_id;
    uint32_t surface_id;
    uint32_t width;
    uint32_t height;
    uint32_t x;
    uint32_t y;
    uint32_t flags;
} SpiceHead;

typedef struct SpiceMsgcDisplayInit {
    uint8_t pixmap_cache_id;
    int64_t pixmap_cache_size;
    uint8_t glz_dictionary_id;
    int32_t glz_dictionary_window_size;
} SpiceMsgcDisplayInit;

typedef struct SpiceMsgcDisplayStreamReport {
    uint32_t stream_id;
    uint32_t unique_id;
    uint32_t start_frame_mm_time;
    uint32_t end_frame_mm_time;
    uint32_t num_frames;
    uint32_t num_drops;
    int32_t last_frame_delay;
    uint32_t audio_delay;
} SpiceMsgcDisplayStreamReport;

typedef struct SpiceMsgcDisplayPreferredCompression {
    uint8_t image_compression;
} SpiceMsgcDisplayPreferredCompression;

typedef struct SpiceMsgcDisplayGlDrawDone {
    char dummy[0];
} SpiceMsgcDisplayGlDrawDone;

typedef struct SpiceMsgcDisplayPreferredVideoCodecType {
    uint8_t num_of_codecs;
    uint8_t codecs[0];
} SpiceMsgcDisplayPreferredVideoCodecType;

typedef struct SpiceMsgDisplayMode {
    uint32_t x_res;
    uint32_t y_res;
    uint32_t bits;
} SpiceMsgDisplayMode;

typedef struct SpiceMsgDisplayCopyBits {
    SpiceMsgDisplayBase base;
    SpicePoint src_pos;
} SpiceMsgDisplayCopyBits;

typedef struct SpiceResourceList {
    uint16_t count;
    SpiceResourceID resources[0];
} SpiceResourceList;

typedef struct SpiceMsgDisplayInvalOne {
    uint64_t id;
} SpiceMsgDisplayInvalOne;

typedef struct SpiceMsgDisplayStreamCreate {
    uint32_t surface_id;
    uint32_t id;
    uint8_t flags;
    uint8_t codec_type;
    uint64_t stamp;
    uint32_t stream_width;
    uint32_t stream_height;
    uint32_t src_width;
    uint32_t src_height;
    SpiceRect dest;
    SpiceClip clip;
} SpiceMsgDisplayStreamCreate;

typedef struct SpiceMsgDisplayStreamData {
    SpiceStreamDataHeader base;
    uint32_t data_size;
    uint8_t data[0];
} SpiceMsgDisplayStreamData;

typedef struct SpiceMsgDisplayStreamClip {
    uint32_t id;
    SpiceClip clip;
} SpiceMsgDisplayStreamClip;

typedef struct SpiceMsgDisplayStreamDestroy {
    uint32_t id;
} SpiceMsgDisplayStreamDestroy;

typedef struct SpiceMsgDisplayDrawFill {
    SpiceMsgDisplayBase base;
    SpiceFill data;
} SpiceMsgDisplayDrawFill;

typedef struct SpiceMsgDisplayDrawOpaque {
    SpiceMsgDisplayBase base;
    SpiceOpaque data;
} SpiceMsgDisplayDrawOpaque;

typedef struct SpiceMsgDisplayDrawCopy {
    SpiceMsgDisplayBase base;
    SpiceCopy data;
} SpiceMsgDisplayDrawCopy;

typedef struct SpiceMsgDisplayDrawBlackness {
    SpiceMsgDisplayBase base;
    SpiceBlackness data;
} SpiceMsgDisplayDrawBlackness;

typedef struct SpiceMsgDisplayDrawWhiteness {
    SpiceMsgDisplayBase base;
    SpiceWhiteness data;
} SpiceMsgDisplayDrawWhiteness;

typedef struct SpiceMsgDisplayDrawInvers {
    SpiceMsgDisplayBase base;
    SpiceInvers data;
} SpiceMsgDisplayDrawInvers;

typedef struct SpiceMsgDisplayDrawRop3 {
    SpiceMsgDisplayBase base;
    SpiceRop3 data;
} SpiceMsgDisplayDrawRop3;

typedef struct SpiceMsgDisplayDrawStroke {
    SpiceMsgDisplayBase base;
    SpiceStroke data;
} SpiceMsgDisplayDrawStroke;

typedef struct SpiceMsgDisplayDrawText {
    SpiceMsgDisplayBase base;
    SpiceText data;
} SpiceMsgDisplayDrawText;

typedef struct SpiceMsgDisplayDrawTransparent {
    SpiceMsgDisplayBase base;
    SpiceTransparent data;
} SpiceMsgDisplayDrawTransparent;

typedef struct SpiceMsgDisplayDrawAlphaBlend {
    SpiceMsgDisplayBase base;
    SpiceAlphaBlend data;
} SpiceMsgDisplayDrawAlphaBlend;

typedef struct SpiceMsgSurfaceCreate {
    uint32_t surface_id;
    uint32_t width;
    uint32_t height;
    uint32_t format;
    uint32_t flags;
} SpiceMsgSurfaceCreate;

typedef struct SpiceMsgSurfaceDestroy {
    uint32_t surface_id;
} SpiceMsgSurfaceDestroy;

typedef struct SpiceMsgDisplayStreamDataSized {
    SpiceStreamDataHeader base;
    uint32_t width;
    uint32_t height;
    SpiceRect dest;
    uint32_t data_size;
    uint8_t data[0];
} SpiceMsgDisplayStreamDataSized;

typedef struct SpiceMsgDisplayMonitorsConfig {
    uint16_t count;
    uint16_t max_allowed;
    SpiceHead heads[0];
} SpiceMsgDisplayMonitorsConfig;

typedef struct SpiceMsgDisplayDrawComposite {
    SpiceMsgDisplayBase base;
    SpiceComposite data;
} SpiceMsgDisplayDrawComposite;

typedef struct SpiceMsgDisplayStreamActivateReport {
    uint32_t stream_id;
    uint32_t unique_id;
    uint32_t max_window_size;
    uint32_t timeout_ms;
} SpiceMsgDisplayStreamActivateReport;

typedef struct SpiceMsgDisplayGlScanoutUnix {
    int drm_dma_buf_fd;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t drm_fourcc_format;
    uint32_t flags;
} SpiceMsgDisplayGlScanoutUnix;

typedef struct SpiceMsgDisplayGlDraw {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} SpiceMsgDisplayGlDraw;

typedef struct SpiceMsgcKeyDown {
    uint32_t code;
} SpiceMsgcKeyDown;

typedef struct SpiceMsgcKeyUp {
    uint32_t code;
} SpiceMsgcKeyUp;

typedef struct SpiceMsgcKeyModifiers {
    uint16_t modifiers;
} SpiceMsgcKeyModifiers;

typedef struct SpiceMsgcMouseMotion {
    int32_t dx;
    int32_t dy;
    uint16_t buttons_state;
} SpiceMsgcMouseMotion;

typedef struct SpiceMsgcMousePosition {
    uint32_t x;
    uint32_t y;
    uint16_t buttons_state;
    uint8_t display_id;
} SpiceMsgcMousePosition;

typedef struct SpiceMsgcMousePress {
    uint8_t button;
    uint16_t buttons_state;
} SpiceMsgcMousePress;

typedef struct SpiceMsgcMouseRelease {
    uint8_t button;
    uint16_t buttons_state;
} SpiceMsgcMouseRelease;

typedef struct SpiceMsgInputsInit {
    uint16_t keyboard_modifiers;
} SpiceMsgInputsInit;

typedef struct SpiceMsgInputsKeyModifiers {
    uint16_t modifiers;
} SpiceMsgInputsKeyModifiers;

typedef struct SpiceCursor {
    uint16_t flags;
    SpiceCursorHeader header;
    uint32_t data_size;
    uint8_t *data;
} SpiceCursor;

typedef struct SpiceMsgCursorInit {
    SpicePoint16 position;
    uint16_t trail_length;
    uint16_t trail_frequency;
    uint8_t visible;
    SpiceCursor cursor;
} SpiceMsgCursorInit;

typedef struct SpiceMsgCursorSet {
    SpicePoint16 position;
    uint8_t visible;
    SpiceCursor cursor;
} SpiceMsgCursorSet;

typedef struct SpiceMsgCursorMove {
    SpicePoint16 position;
} SpiceMsgCursorMove;

typedef struct SpiceMsgCursorTrail {
    uint16_t length;
    uint16_t frequency;
} SpiceMsgCursorTrail;

typedef struct SpiceMsgPlaybackPacket {
    uint32_t time;
    uint32_t data_size;
    uint8_t *data;
} SpiceMsgPlaybackPacket;

typedef struct SpiceMsgPlaybackMode {
    uint32_t time;
    uint16_t mode;
    uint32_t data_size;
    uint8_t *data;
} SpiceMsgPlaybackMode;

typedef struct SpiceMsgPlaybackStart {
    uint32_t channels;
    uint16_t format;
    uint32_t frequency;
    uint32_t time;
} SpiceMsgPlaybackStart;

typedef struct SpiceMsgAudioVolume {
    uint8_t nchannels;
    uint16_t volume[0];
} SpiceMsgAudioVolume;

typedef struct SpiceMsgAudioMute {
    uint8_t mute;
} SpiceMsgAudioMute;

typedef struct SpiceMsgPlaybackLatency {
    uint32_t latency_ms;
} SpiceMsgPlaybackLatency;

typedef struct SpiceMsgcRecordStartMark {
    uint32_t time;
} SpiceMsgcRecordStartMark;

typedef struct SpiceMsgRecordStart {
    uint32_t channels;
    uint16_t format;
    uint32_t frequency;
} SpiceMsgRecordStart;

typedef struct SpiceMsgcPortEvent {
    uint8_t event;
} SpiceMsgcPortEvent;

typedef struct SpiceMsgPortInit {
    uint32_t name_size;
    uint8_t *name;
    uint8_t opened;
} SpiceMsgPortInit;

typedef struct SpiceMsgPortEvent {
    uint8_t event;
} SpiceMsgPortEvent;


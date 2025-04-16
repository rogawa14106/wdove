#ifndef _WD_TLS_H
#define _WD_TLS_H

#include <sys/types.h>

typedef struct u_int24 {
  unsigned int int24 : 24;
} u_int24_t;

// Alert
typedef enum {
  warning = (1),
  fatal = (2),
  //(255)
} AlertLevel;

typedef enum {
  close_notify = (0),
  unexpected_message = (10),
  bad_record_mac = (20),
  decryption_failed_RESERVED = (21),
  record_overflow = (22),
  decompression_failure_RESERVED = (30),
  handshake_failure = (40),
  no_certificate_RESERVED = (41),
  bad_certificate = (42),
  unsupported_certificate = (43),
  certificate_revoked = (44),
  certificate_expired = (45),
  certificate_unknown = (46),
  illegal_parameter = (47),
  unknown_ca = (48),
  access_denied = (49),
  decode_error = (50),
  decrypt_error = (51),
  export_restriction_RESERVED = (60),
  protocol_version = (70),
  insufficient_security = (71),
  internal_error = (80),
  inappropriate_fallback = (86),
  user_canceled = (90),
  no_renegotiation_RESERVED = (100),
  missing_extension = (109),
  unsupported_extension = (110),
  certificate_unobtainable_RESERVED = (111),
  unrecognized_name = (112),
  bad_certificate_status_response = (113),
  bad_certificate_hash_value_RESERVED = (114),
  unknown_psk_identity = (115),
  certificate_required = (116),
  no_application_protocol = (120),
  //  (255)
} AlertDescription;

typedef struct {
  AlertLevel level;
  AlertDescription description;
} Alert;

// Content Type
typedef enum {
  handshake = (22),
  // (255)
} ContentType;

// Handshake Type
typedef enum {
  hello_request_RESERVED = (0),
  client_hello = (1),
  server_hello = (2),
  hello_verify_request_RESERVED = (3),
  new_session_ticket = (4),
  end_of_early_data = (5),
  hello_retry_request_RESERVED = (6),
  encrypted_extensions = (8),
  certificate = (11),
  server_key_exchange_RESERVED = (12),
  certificate_request = (13),
  server_hello_done_RESERVED = (14),
  certificate_verify = (15),
  client_key_exchange_RESERVED = (16),
  finished = (20),
  certificate_url_RESERVED = (21),
  certificate_status_RESERVED = (22),
  supplemental_data_RESERVED = (23),
  key_update = (24),
  message_hash = (254),
  // _ = (255)
} HandshakeType;

// Extension Type
typedef enum {
  server_name = (0),                             /* RFC 6066 */
  max_fragment_length = (1),                     /* RFC 6066 */
  status_request = (5),                          /* RFC 6066 */
  supported_groups = (10),                       /* RFC 8422, 7919 */
  signature_algorithms = (13),                   /* RFC 8446 */
  use_srtp = (14),                               /* RFC 5764 */
  heartbeat = (15),                              /* RFC 6520 */
  application_layer_protocol_negotiation = (16), /* RFC 7301 */
  signed_certificate_timestamp = (18),           /* RFC 6962 */
  client_certificate_type = (19),                /* RFC 7250 */
  server_certificate_type = (20),                /* RFC 7250 */
  padding = (21),                                /* RFC 7685 */
  pre_shared_key = (41),                         /* RFC 8446 */
  early_data = (42),                             /* RFC 8446 */
  supported_versions = (43),                     /* RFC 8446 */
  cookie = (44),                                 /* RFC 8446 */
  psk_key_exchange_modes = (45),                 /* RFC 8446 */
  certificate_authorities = (47),                /* RFC 8446 */
  oid_filters = (48),                            /* RFC 8446 */
  post_handshake_auth = (49),                    /* RFC 8446 */
  signature_algorithms_cert = (50),              /* RFC 8446 */
  key_share = (51),                              /* RFC 8446 */
  // _ = (65535),
} ExtensionType;

typedef struct {
  ExtensionType extension_type;
  // opaque extension_data;
} Extension;

/*
 * Hand shake
 *
 */
// Client Hello
// typedef u_int16_t ProtocolVersion;
typedef enum {
  TLS_VERSION_1_2 = 0x0303,
  //
} wd_tls_version_t;

#define SSL_MAX_SSL_SESSION_ID_LENGTH 32

// typedef u_char Random[32];       // opaque
typedef u_int8_t CipherSuite[2]; /* Cryptographic suite selector */
typedef struct {
  // TLS_VERSION_1_2 fixed TLS1.2 evn if using TLS1.3
  wd_tls_version_t legacy_version;
  u_char random[32];
  size_t session_id_len;
  u_char legacy_session_id[SSL_MAX_SSL_SESSION_ID_LENGTH];
  size_t cipher_suites_len;
  CipherSuite *cipher_suites;
  size_t legacy_compression_len;
  u_char legacy_compression_methods; // NULL
  Extension extensions;              // TODO
} wd_tls_client_hello_t;

int wd_tls_client_hello();
int wd_tls_parse_server_hello();

#endif /* ifndef _WD_TLS_H */

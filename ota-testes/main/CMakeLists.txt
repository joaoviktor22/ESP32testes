# Embed the server root certificate into the final binary
idf_build_get_property(project_dir PROJECT_DIR)
idf_component_register(SRCS "ota_json.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common) 
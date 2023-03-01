# Embed the server root certificate into the final binary
idf_build_get_property(project_dir PROJECT_DIR)
idf_component_register(SRCS "main.c" "ota_json.c" "blufi_security.c" "blufi_main.c" "blufi_init.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
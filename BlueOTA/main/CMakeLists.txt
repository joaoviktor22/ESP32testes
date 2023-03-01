# Embed the server root certificate into the final binary
idf_build_get_property(project_dir PROJECT_DIR)
idf_component_register(SRCS "main.c" "ota_json.c" "bluetooth.c" "blufi_example_main.c" "blufi_init.c" "blufi_security.c" 
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)
component_compile_options(-Wno-error=format= -Wno-format)

set(COMPONENT_SRCS main.c MyWiFi.c OTAServer.c)
set(COMPONENT_ADD_INCLUDEDIRS ".")

set(COMPONENT_EMBED_TXTFILES WebsiteFiles/index.html WebsiteFiles/favicon.ico WebsiteFiles/jquery-3.4.1.min.js)

register_component()


# Create a SPIFFS image from the contents of the 'spiffs_content' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'. 
spiffs_create_partition_image(storage ../spiffs_content FLASH_IN_PROJECT)

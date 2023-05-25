set(COMPONENT_SRCS
	"app_main.c"
	"mqtt/mqtt_base.c"
	"sensors/app_sensors.c"
	"mqtt/app_mqtt.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"includes"
	"${CMAKE_CURRENT_LIST_DIR}"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )
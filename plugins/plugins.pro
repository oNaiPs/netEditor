#DEFINES += QT_NO_DEBUG_STREAM

TEMPLATE = subdirs

SUBDIRS +=  export/example_export \
			export/sumo_export \
			export/feup_simulator_export \
			#import/gdal_import \
			#import/osm_import \
			#import/pg_import \
			import/example_import \
			import/sumo_import
			
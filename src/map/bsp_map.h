#ifndef BSP_MAP_H_INCLUDED
#define BSP_MAP_H_INCLUDED

#include "base_map.h"
#include "bsp_data.h"

class bsp_map : public base_map {
	protected:
		bsp_data m_bsp_data;
};

#endif // BSP_MAP_H_INCLUDED
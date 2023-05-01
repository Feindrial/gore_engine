#include "memory/render_data_storage.h"

namespace gore
{
    render_data_storage *render_data_storage::singleton_ = nullptr;

    render_data_storage::render_data_storage()
    {
        singleton_ = this;
    }
    render_data_storage::~render_data_storage()
    {
        
    }
}
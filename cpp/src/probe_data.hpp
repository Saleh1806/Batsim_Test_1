#pragma once

#include <memory>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class ProbeData
    {
    public:
        ~ProbeData() = default;

        static std::shared_ptr<ProbeData> make_aggregated(double data);
        static std::shared_ptr<ProbeData> make_vectorial(
            const std::shared_ptr<std::vector<double> > & data);

        ProbeData & set_resources_as_hosts(const std::string & intervalset);
        ProbeData & set_resources_as_links(const std::shared_ptr<std::vector<std::string> > & links);
    private:
        friend class MessageBuilder;
        ProbeData() = default;

        fb::ProbeData _type = fb::ProbeData_NONE;
        std::shared_ptr<std::vector<double> > _vectorial_data = nullptr;
        double _aggregated_data = 0.0;

        fb::Resources _resources_type = fb::Resources_NONE;
        std::string _hosts_resources;
        std::shared_ptr<std::vector<std::string> > _links_resources = nullptr;
    };
} // end of namespace batprotocol

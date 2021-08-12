#include "probe_data.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<ProbeData> ProbeData::make_aggregated(double data)
{
    std::shared_ptr<ProbeData> probe_data(new ProbeData());
    probe_data->_type = fb::ProbeData_AggregatedProbeData;
    probe_data->_aggregated_data = data;

    return probe_data;
}

std::shared_ptr<ProbeData> ProbeData::make_vectorial(
    const std::shared_ptr<std::vector<double> > & data)
{
    std::shared_ptr<ProbeData> probe_data(new ProbeData());
    probe_data->_type = fb::ProbeData_VectorialProbeData;
    probe_data->_vectorial_data = data;

    return probe_data;
}

ProbeData & ProbeData::set_resources_as_hosts(const std::string & intervalset)
{
    BAT_ENFORCE(_resources_type == fb::Resources_NONE, "Cannot call set_resources_as_hosts since resources have already been set");
    _resources_type = fb::Resources_HostResources;
    _hosts_resources = intervalset;

    return *this;
}

ProbeData & ProbeData::set_resources_as_links(
    const std::shared_ptr<std::vector<std::string> > & links)
{
    BAT_ENFORCE(_resources_type == fb::Resources_NONE, "Cannot call set_resources_as_hosts since resources have already been set");
    BAT_ENFORCE(links.get() != nullptr, "invalid (null) links received");
    _resources_type = fb::Resources_LinkResources;
    _links_resources = links;

    return *this;
}

} // end of namespace batprotocol

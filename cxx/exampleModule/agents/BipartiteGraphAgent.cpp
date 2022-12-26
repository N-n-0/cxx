#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/keynodes.hpp"
#include "BipartiteGraphAgent.hpp"
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_iterator.hpp"
#include "sc-memory/sc_type.hpp"
#include "sc-memory/utils/sc_log.hpp"
#include <iostream>
#include <vector>

using namespace exampleModule;
using namespace std;
using namespace utils;


SC_AGENT_IMPLEMENTATION(BipartiteGraphAgent)
{
    ScMemoryContext *context = new ScMemoryContext(sc_access_lvl_make_max, "example");
    GraphBipartyleFinder bipartyle_finder(context, graph_name);

    bool result = bipartyle_finder.Find();

    if (result)
    {
        std::cout << "Graph " << graph_name << " is bipartite:" << std::endl << std::endl;
        bipartyle_finder.Print();
        std::cout << "****************" << std::endl << std::endl;
    }
    else
    {
        std::cout << "Graph " << graph_name << " is not bipartite" << std::endl;
    }
    
}


//  Copyright (c) 2011 Matthew Anderson
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_226CC70A_D748_4ADA_BB55_70F85566B5CC)
#define HPX_226CC70A_D748_4ADA_BB55_70F85566B5CC

#include <vector>
#include <queue>

#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include "graph_generator.hpp"
#include "splittable_mrg.hpp"
#include "../../array.hpp"

struct vertex_data
{
  std::size_t node;
  std::vector<std::size_t> neighbors;

  vertex_data() {}

  private:
    // serialization support
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & node & neighbors;
    }

};

void make_random_numbers(
       /* in */ int64_t nvalues    /* Number of values to generate */,
       /* in */ uint64_t userseed1 /* Arbitrary 64-bit seed value */,
       /* in */ uint64_t userseed2 /* Arbitrary 64-bit seed value */,
       /* in */ int64_t position   /* Start index in random number stream */,
       /* out */ double* result    /* Returned array of values */
);

///////////////////////////////////////////////////////////////////////////////
namespace graph500 { namespace server
{

    ///////////////////////////////////////////////////////////////////////////
    class HPX_COMPONENT_EXPORT point
      : public hpx::components::managed_component_base<point>
    {
    public:
        point()
        {}

        ///////////////////////////////////////////////////////////////////////
        // Exposed functionality of this component.

        void init(std::size_t objectid,std::size_t scale,std::size_t number_partitions,
                  std::vector<hpx::naming::id_type> const& point_components);

        void bfs();

        std::vector<vertex_data> merge_graph(std::vector<vertex_data> const& data);

        void reset();
    
        bool has_edge(std::size_t edge);

        // Each of the exposed functions needs to be encapsulated into an
        // action type, generating all required boilerplate code for threads,
        // serialization, etc.

        /// Action codes.
        enum actions
        {
            point_init = 0,
            point_bfs = 1,
            point_merge_graph = 2,
            point_reset = 3,
            point_has_edge = 4
        };

        typedef hpx::actions::action4<
            // Component server type.
            point,
            // Action code.
            point_init,
            // Arguments of this action.
            std::size_t,
            std::size_t,
            std::size_t,
            std::vector<hpx::naming::id_type> const&,
            // Method bound to this action.
            &point::init
        > init_action;

        typedef hpx::actions::action0<
            // Component server type.
            point,
            // Action code.
            point_bfs,
            // Arguments of this action.
            // Method bound to this action.
            &point::bfs
        > bfs_action;

        typedef hpx::actions::result_action1<
            // Component server type.
            point,
            std::vector<vertex_data>,
            // Action code.
            point_merge_graph,
            // Arguments of this action.
            std::vector<vertex_data> const&,
            // Method bound to this action.
            &point::merge_graph
        > merge_graph_action;

        typedef hpx::actions::action0<
            // Component server type.
            point,
            // Action code.
            point_reset,
            // Arguments of this action.
            // Method bound to this action.
            &point::reset
        > reset_action;

        typedef hpx::actions::result_action1<
            // Component server type.
            point,
            // Return type.
            bool,
            // Action code.
            point_has_edge,
            // Arguments of this action.
            std::size_t,
            // Method bound to this action.
            &point::has_edge
        > has_edge_action;

    private:
        hpx::lcos::local_mutex mtx_;
        std::size_t idx_;
        std::size_t size_est_;
        std::vector< std::vector<std::size_t> > neighbors_;
        array<std::size_t> parent_;
        std::size_t minnode_;
        std::vector<packed_edge> local_edges_;
        std::vector<std::size_t> reset_list_;
        std::vector<hpx::naming::id_type> point_components_;
    };
}}

// Declaration of serialization support for the actions
HPX_REGISTER_ACTION_DECLARATION_EX(
    graph500::server::point::init_action,
    graph500_point_init_action);

HPX_REGISTER_ACTION_DECLARATION_EX(
    graph500::server::point::bfs_action,
    graph500_point_bfs_action);

HPX_REGISTER_ACTION_DECLARATION_EX(
    graph500::server::point::merge_graph_action,
    graph500_point_merge_graph_action);

HPX_REGISTER_ACTION_DECLARATION_EX(
    graph500::server::point::reset_action,
    graph500_point_reset_action);

HPX_REGISTER_ACTION_DECLARATION_EX(
    graph500::server::point::has_edge_action,
    graph500_point_has_edge_action);

HPX_REGISTER_ACTION_DECLARATION_EX(
    hpx::lcos::base_lco_with_value<std::vector<std::size_t> >::get_value_action,
    get_value_action_vector_size_t);

HPX_REGISTER_ACTION_DECLARATION_EX(
    hpx::lcos::base_lco_with_value<std::vector<std::size_t>>::set_result_action,
    set_result_action_vector_size_t);

#endif


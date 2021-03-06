/*

Copyright (c) 2013, Project OSRM, Dennis Luxen, others
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PHANTOM_NODES_H
#define PHANTOM_NODES_H

#include <osrm/Coordinate.h>
#include "../Util/SimpleLogger.h"
#include "../typedefs.h"

struct PhantomNode
{
    PhantomNode() :
        forward_node_id(SPECIAL_NODEID),
        reverse_node_id(SPECIAL_NODEID),
        name_id(std::numeric_limits<unsigned>::max()),
        forward_weight(INVALID_EDGE_WEIGHT),
        reverse_weight(INVALID_EDGE_WEIGHT),
        forward_offset(0),
        reverse_offset(0),
        packed_geometry_id(SPECIAL_EDGEID),
        fwd_segment_position(0)
    { }

    NodeID forward_node_id;
    NodeID reverse_node_id;
    unsigned name_id;
    int forward_weight;
    int reverse_weight;
    int forward_offset;
    int reverse_offset;
    unsigned packed_geometry_id;
    FixedPointCoordinate location;
    unsigned short fwd_segment_position;

    int GetForwardWeightPlusOffset() const
    {
        if (SPECIAL_NODEID == forward_node_id)
        {
            return 0;
        }
        const int result = (forward_offset + forward_weight);
        return result;
    }

    int GetReverseWeightPlusOffset() const
    {
        if (SPECIAL_NODEID == reverse_node_id)
        {
            return 0;
        }
        const int result = (reverse_offset + reverse_weight);
        return result;
    }

    void Reset()
    {
        forward_node_id = SPECIAL_NODEID;
        name_id = SPECIAL_NODEID;
        forward_weight = INVALID_EDGE_WEIGHT;
        reverse_weight = INVALID_EDGE_WEIGHT;
        forward_offset = 0;
        reverse_offset = 0;
        location.Reset();
    }

    bool isBidirected() const
    {
        return (forward_node_id != SPECIAL_NODEID) &&
               (reverse_node_id != SPECIAL_NODEID);
    }

    bool IsCompressed() const
    {
        return (forward_offset != 0) || (reverse_offset != 0);
    }

    bool isValid(const unsigned numberOfNodes) const
    {
        return
            location.isValid() &&
            (
                (forward_node_id < numberOfNodes) ||
                (reverse_node_id < numberOfNodes)
            ) &&
            (
                (forward_weight != INVALID_EDGE_WEIGHT) ||
                (reverse_weight != INVALID_EDGE_WEIGHT)
            ) &&
            (name_id != std::numeric_limits<unsigned>::max()
        );
    }

    bool isValid() const
    {
        return location.isValid() &&
               (name_id != std::numeric_limits<unsigned>::max());
    }

    bool operator==(const PhantomNode & other) const
    {
        return location == other.location;
    }
};

struct PhantomNodes
{
    PhantomNode source_phantom;
    PhantomNode target_phantom;

    void Reset()
    {
        source_phantom.Reset();
        target_phantom.Reset();
    }

    bool PhantomsAreOnSameNodeBasedEdge() const
    {
        return (source_phantom.forward_node_id == target_phantom.forward_node_id);
    }

    bool AtLeastOnePhantomNodeIsInvalid() const
    {
        return ((source_phantom.forward_node_id == SPECIAL_NODEID) && (source_phantom.reverse_node_id == SPECIAL_NODEID)) ||
               ((target_phantom.forward_node_id == SPECIAL_NODEID) && (target_phantom.reverse_node_id == SPECIAL_NODEID));
    }

    bool PhantomNodesHaveEqualLocation() const
    {
        return source_phantom == target_phantom;
    }
};

inline std::ostream& operator<<(std::ostream &out, const PhantomNodes & pn)
{
    out << "source_coord: " << pn.source_phantom.location        << "\n";
    out << "target_coord: " << pn.target_phantom.location        << std::endl;
    return out;
}

inline std::ostream& operator<<(std::ostream &out, const PhantomNode & pn)
{
    out <<  "node1: " << pn.forward_node_id      << ", " <<
            "node2: " << pn.reverse_node_id      << ", " <<
            "name: "  << pn.name_id              << ", " <<
            "fwd-w: " << pn.forward_weight       << ", " <<
            "rev-w: " << pn.reverse_weight       << ", " <<
            "fwd-o: " << pn.forward_offset       << ", " <<
            "rev-o: " << pn.reverse_offset       << ", " <<
            "geom: "  << pn.packed_geometry_id   << ", " <<
            "pos: "   << pn.fwd_segment_position << ", " <<
            "loc: "   << pn.location;
    return out;
}

#endif // PHANTOM_NODES_H

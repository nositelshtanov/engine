#include "Edge3dObj.h"

void Edge3dObj::SetBegVertex(const MVertex3D& vertex) {
    m_edge.SetBegVertex(vertex);
}

void Edge3dObj::SetEndVertex(const MVertex3D& vertex) {
    m_edge.SetEndVertex(vertex);
}

std::vector<MEdge3D> Edge3dObj::GetEdges() const {
    return {m_edge};
}

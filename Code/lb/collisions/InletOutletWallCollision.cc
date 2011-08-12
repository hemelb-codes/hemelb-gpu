#include "lb/collisions/InletOutletWallCollision.h"
#include "lb/collisions/CollisionVisitor.h"

namespace hemelb
{
  namespace lb
  {
    namespace collisions
    {
      InletOutletWallCollision::InletOutletWallCollision(BoundaryComms* iBoundaryComms) :
        mBoundaryComms(iBoundaryComms)
      {

      }

      InletOutletWallCollision::~InletOutletWallCollision()
      {

      }

      distribn_t InletOutletWallCollision::getBoundaryDensityArray(const int index)
      {
        return mBoundaryComms->GetBoundaryDensity(index);
      }

      void InletOutletWallCollision::AcceptCollisionVisitor(CollisionVisitor* v,
                                                            const bool iDoRayTracing,
                                                            const site_t iFirstIndex,
                                                            const site_t iSiteCount,
                                                            const LbmParameters* iLbmParams,
                                                            geometry::LatticeData* bLatDat,
                                                            hemelb::vis::Control *iControl)
      {
        v->VisitInletOutletWall(this,
                                iDoRayTracing,
                                iFirstIndex,
                                iSiteCount,
                                iLbmParams,
                                bLatDat,
                                iControl);
      }

    }
  }
}

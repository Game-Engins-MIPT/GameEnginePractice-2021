#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "RenderEngine.h"

void register_ecs_mesh_systems(flecs::world &ecs)
{
  ecs.system<const MeshName>()
    .each([&](flecs::entity e, const MeshName &nm)
    {
      ecs.query<RenderEnginePtr>()
        .each([&](RenderEnginePtr re)
        {
          if (re.ptr->IsInited())
          {
            uint32_t index = re.ptr->GetRT()->RC_CreateSceneNode("ogrehead.mesh Imported");
            e.set(SceneNodeDeliveryIndex{index});
            e.remove<MeshName>();
          }
        });
    });

  ecs.system<const SceneNodeDeliveryIndex, const Position>()
    .each([&](flecs::entity e, const SceneNodeDeliveryIndex &delivery_index, const Position &pos)
    {
      ecs.query<RenderEnginePtr>()
        .each([&](RenderEnginePtr re)
        {
          if (re.ptr->IsInited())
          {
            std::map<uint32_t, void*> delivery = re.ptr->GetRT()->GetDeliveryQueue();
            auto fiter = delivery.find(delivery_index.idx);
            if (fiter == delivery.end())
              return;
            Ogre::SceneNode *pSceneNode = (Ogre::SceneNode*)fiter->second;
            re.ptr->GetRT()->RC_SceneNodeSetPosition(pSceneNode, pos);
            e.set<SceneNode>(SceneNode{pSceneNode});
            e.remove<SceneNodeDeliveryIndex>();
          }
        });
    });

  ecs.system<SceneNode, const Position>()
    .each([&](SceneNode &scene_node, const Position &pos)
    {
      ecs.query<RenderEnginePtr>()
        .each([&](RenderEnginePtr re)
        {
          re.ptr->GetRT()->RC_SceneNodeSetPosition(scene_node.pSceneNode, pos);
        });
    });
}


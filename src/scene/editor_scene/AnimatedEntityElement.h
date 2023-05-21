#ifndef ANIMATED_ENTITY_ELEMENT_H
#define ANIMATED_ENTITY_ELEMENT_H

#include "SceneElement.h"
#include "scene/SceneContext.h"

struct Keyframe {
    float time; // The time of this keyframe
    glm::quat rotation; // The rotation at this keyframe
};

struct Animation {
    std::vector<Keyframe> keyframes;
};


namespace EditorScene {
    class AnimatedEntityElement : virtual public SceneElement, public LocalTransformComponent, public LitMaterialComponent, public AnimationComponent {
    private:
        std::unordered_map<std::string, Animation> animations; // A map of animations
    public:
        /// NOTE: Must be unique per element type, as it is used to select generators,
        ///       so if you are creating a new element type make sure to change this to a new unique name.
        static constexpr const char* ELEMENT_TYPE_NAME = "Animated Entity";

        std::shared_ptr<AnimatedEntityRenderer::Entity> rendered_entity;

        AnimationParameters animation_parameters{};

        AnimatedEntityElement(const ElementRef& parent, std::string name, const glm::vec3& position, const glm::vec3& euler_rotation, const glm::vec3& scale, std::shared_ptr<AnimatedEntityRenderer::Entity> rendered_entity) :
            SceneElement(parent, std::move(name)), LocalTransformComponent(position, euler_rotation, scale), LitMaterialComponent(rendered_entity->instance_data.material), AnimationComponent(), rendered_entity(std::move(rendered_entity)) {}

        static std::unique_ptr<AnimatedEntityElement> new_default(const SceneContext& scene_context, ElementRef parent);
        static std::unique_ptr<AnimatedEntityElement> from_json(const SceneContext& scene_context, ElementRef parent, const json& j);
        [[nodiscard]] json into_json() const override;

        void add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) override;

        void update_instance_data() override;
        
        void add_to_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.insert_entity(rendered_entity);
        }

        void remove_from_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.remove_entity(rendered_entity);
        }

        void addAnimation(const std::string& name, const Animation& animation);

        [[nodiscard]] std::shared_ptr<AnimatedEntityInterface> get_entity() override;
        [[nodiscard]] AnimationParameters& get_animation_parameters() override;

        [[nodiscard]] const char* element_type_name() const override;
    };
}

#endif //ANIMATED_ENTITY_ELEMENT_H

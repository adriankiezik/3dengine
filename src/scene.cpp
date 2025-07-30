#include "scene.h"
#include <iostream>
#include "framebuffer.h"
#include <nlohmann/json.hpp>

Scene::Scene(
    Camera &camera,
    Framebuffer &framebuffer) : camera(camera), framebuffer(framebuffer),
                                grid(1.5f, "../shaders/grid_vertex_shader.glsl", "../shaders/grid_fragment_shader.glsl") {}

void Scene::addObject(const Object &object)
{
  objects.push_back(object);
}

void Scene::update()
{
  framebuffer.bind();

  grid.render(glm::mat4(1.0f), camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPosition(), 100, glm::vec3(0.4f, 0.6f, 0.8f));

  // Frustum culling optimization
  glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
  Frustum frustum = extractFrustum(viewProjectionMatrix);

  // Only render objects that are visible in the camera frustum
  for (Object &object : objects)
  {
    if (isInFrustum(frustum, object.getPosition(), 5.0f)) // 5.0f is a rough bounding radius
    {
      object.Draw(camera.getPosition(), camera.getViewMatrix(), camera.getProjectionMatrix());
    }
  }

  framebuffer.unbind();
}

nlohmann::json Scene::toJson() const
{
  nlohmann::json sceneData;
  sceneData["objects"] = nlohmann::json::array();

  const auto &objects = getObjects();
  for (size_t i = 0; i < objects.size(); ++i)
  {
    const auto &obj = objects[i];
    nlohmann::json objData;

    // Object transform properties
    objData["name"] = obj.getName();
    objData["position"] = {
        obj.getPosition().x,
        obj.getPosition().y,
        obj.getPosition().z};
    objData["rotation"] = {
        obj.getRotation().x,
        obj.getRotation().y,
        obj.getRotation().z};
    objData["scale"] = {
        obj.getScale().x,
        obj.getScale().y,
        obj.getScale().z};

    // Model properties
    const Model &model = obj.getModel();
    objData["model"] = {
        {"modelPath", model.getModelPath()},
        {"vertexShader", model.getVertexShaderPath()},
        {"fragmentShader", model.getFragmentShaderPath()},
        {"textures", nlohmann::json::array()}};

    // Add textures
    size_t textureIndex = 0;
    for (const auto &texture : model.getTexturePaths())
    {
      objData["model"]["textures"][textureIndex++] = {
          {"type", texture.first},
          {"path", texture.second}};
    }

    // Add scripts
    objData["scripts"] = nlohmann::json::array();
    for (const auto &script : obj.getScripts())
    {
      objData["scripts"].push_back(script.path);
    }

    sceneData["objects"][i] = objData;
  }

  return sceneData;
}

void Scene::fromJson(const nlohmann::json &json)
{
  clear();

  if (!json.contains("scene") || !json["scene"].is_object())
    return;

  const auto &objectsData = json["scene"]["objects"];
  if (!objectsData.is_array())
    return;

  for (const auto &objData : objectsData)
  {
    try
    {
      // Extract model data
      const auto &modelData = objData["model"];
      std::vector<std::pair<std::string, std::string>> textures;

      // Load textures
      for (const auto &textureData : modelData["textures"])
      {
        textures.push_back({textureData["type"].get<std::string>(),
                            textureData["path"].get<std::string>()});
      }

      // Create model
      Model model(modelData["modelPath"].get<std::string>(),
                  textures,
                  modelData["vertexShader"].get<std::string>(),
                  modelData["fragmentShader"].get<std::string>());

      // Create object with transform data
      Object obj(model,
                 glm::vec3(objData["position"][0], objData["position"][1], objData["position"][2]),
                 glm::vec3(objData["rotation"][0], objData["rotation"][1], objData["rotation"][2]),
                 glm::vec3(objData["scale"][0], objData["scale"][1], objData["scale"][2]),
                 objData["name"]);

      // Add scripts if they exist
      if (objData.contains("scripts") && objData["scripts"].is_array())
      {
        for (const auto &scriptData : objData["scripts"])
        {
          Script script(scriptData.get<std::string>());
          obj.addScript(script);
        }
      }

      addObject(obj);
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error loading object: " << e.what() << std::endl;
      continue;
    }
  }
}

void Scene::clear()
{
  objects.clear();
}

Frustum Scene::extractFrustum(const glm::mat4& viewProjection)
{
  Frustum frustum;
  
  // Extract frustum planes from view-projection matrix
  // Left plane
  frustum.planes[0] = glm::vec4(
    viewProjection[0][3] + viewProjection[0][0],
    viewProjection[1][3] + viewProjection[1][0],
    viewProjection[2][3] + viewProjection[2][0],
    viewProjection[3][3] + viewProjection[3][0]
  );
  
  // Right plane
  frustum.planes[1] = glm::vec4(
    viewProjection[0][3] - viewProjection[0][0],
    viewProjection[1][3] - viewProjection[1][0],
    viewProjection[2][3] - viewProjection[2][0],
    viewProjection[3][3] - viewProjection[3][0]
  );
  
  // Bottom plane
  frustum.planes[2] = glm::vec4(
    viewProjection[0][3] + viewProjection[0][1],
    viewProjection[1][3] + viewProjection[1][1],
    viewProjection[2][3] + viewProjection[2][1],
    viewProjection[3][3] + viewProjection[3][1]
  );
  
  // Top plane
  frustum.planes[3] = glm::vec4(
    viewProjection[0][3] - viewProjection[0][1],
    viewProjection[1][3] - viewProjection[1][1],
    viewProjection[2][3] - viewProjection[2][1],
    viewProjection[3][3] - viewProjection[3][1]
  );
  
  // Near plane
  frustum.planes[4] = glm::vec4(
    viewProjection[0][3] + viewProjection[0][2],
    viewProjection[1][3] + viewProjection[1][2],
    viewProjection[2][3] + viewProjection[2][2],
    viewProjection[3][3] + viewProjection[3][2]
  );
  
  // Far plane
  frustum.planes[5] = glm::vec4(
    viewProjection[0][3] - viewProjection[0][2],
    viewProjection[1][3] - viewProjection[1][2],
    viewProjection[2][3] - viewProjection[2][2],
    viewProjection[3][3] - viewProjection[3][2]
  );
  
  // Normalize planes
  for (int i = 0; i < 6; i++) {
    float length = glm::length(glm::vec3(frustum.planes[i]));
    frustum.planes[i] /= length;
  }
  
  return frustum;
}

bool Scene::isInFrustum(const Frustum& frustum, const glm::vec3& position, float radius)
{
  // Check if sphere (position + radius) is inside all frustum planes
  for (int i = 0; i < 6; i++) {
    float distance = glm::dot(glm::vec3(frustum.planes[i]), position) + frustum.planes[i].w;
    if (distance < -radius) {
      return false; // Object is completely outside this plane
    }
  }
  return true; // Object is inside or intersecting the frustum
}

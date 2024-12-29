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

  for (Object &object : objects)
  {
    object.Draw(camera.getPosition(), camera.getViewMatrix(), camera.getProjectionMatrix());
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

#pragma once

#include "App.hpp"

#include <array>
#include <memory>
#include <vector>

class WaterSurfaceParticle {
  private:
    float m_TargetHeight = 0;

  public:
    float velocity = 0;
    float height = 0;

    void update(float dampening, float tension) {
        float x = height - m_TargetHeight;
        float acceleration = -tension * x - dampening * velocity;

        height += velocity;
        velocity += acceleration;
    }
};

template <size_t N> class WaterSurface {
  private:
    std::array<WaterSurfaceParticle, N> m_Springs;
    std::array<float, N> m_LeftDeltas;
    std::array<float, N> m_RightDeltas;

    std::shared_ptr<Engine::Entity> m_Entity;

    float m_Spread = 0.25;
    float m_Dampening = 0.025;
    float m_Tension = 0.015;

  public:
    WaterSurface() {
        m_LeftDeltas.fill(0);
        m_RightDeltas.fill(0);
    }

    void init(Context *context) {
        std::vector<float> vertices(N * 4 * 6);

        std::vector<uint32_t> indexes;
        for (size_t i = 0; i < N - 1; i++) {
            indexes.push_back(i * 2);
            indexes.push_back(i * 2 + 1);
            indexes.push_back(i * 2 + 2);
            indexes.push_back(i * 2 + 1);
            indexes.push_back(i * 2 + 3);
            indexes.push_back(i * 2 + 2);
        }

        m_Entity = context->addEntity("water");
        m_Entity->addComponent<Engine::LocationComponent>(0, 220);
        m_Entity->addComponent<Engine::RenderComponent>(vertices, indexes, 400,
                                                        400);
        m_Entity->addComponent<Engine::MaterialComponent>();
        m_Entity->addComponent<Engine::IgnoreLightComponent>();
    }

    void update() {
        for (int i = 0; i < N; i++) {
            m_Springs[i].update(m_Dampening, m_Tension);
        }

        for (int j = 0; j < 1; j++) {
            for (int i = 0; i < N; i++) {
                if (i > 0) {
                    float dh = m_Springs[i].height - m_Springs[i - 1].height;
                    m_LeftDeltas[i] = m_Spread * dh;
                    m_Springs[i - 1].velocity += m_LeftDeltas[i];
                }
                if (i < N - 1) {
                    float dh = m_Springs[i].height - m_Springs[i + 1].height;
                    m_RightDeltas[i] = m_Spread * dh;
                    m_Springs[i + 1].velocity += m_RightDeltas[i];
                }
            }

            for (int i = 0; i < N; i++) {
                if (i > 0)
                    m_Springs[i - 1].height += m_LeftDeltas[i];
                if (i < N - 1)
                    m_Springs[i + 1].height += m_RightDeltas[i];
            }
        }

        auto render = m_Entity->getComponent<Engine::RenderComponent>();
        float dw = 2.0 / N;

        std::vector<float> vertices;
        for (float i = 0; i < N; i++) {
            // 0
            vertices.push_back(-1.0 + dw * i);
            vertices.push_back(m_Springs[i].height);
            vertices.push_back(0.0);

            vertices.push_back(0.2);
            vertices.push_back(0.2);
            vertices.push_back(0.2);
            vertices.push_back(0.5);

            // 1
            vertices.push_back(-1.0 + dw * i);
            vertices.push_back(-1.0);
            vertices.push_back(0.0);

            vertices.push_back(0.0);
            vertices.push_back(0.0);
            vertices.push_back(0.0);
            vertices.push_back(0.5);
        }

        render->vertexBuffer->updateVertices(vertices);
    }

    void splash(int index, float velocity) {
        m_Springs[index].velocity = velocity;
    }
};
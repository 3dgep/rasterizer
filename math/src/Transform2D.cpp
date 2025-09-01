#include <math/Transform2D.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/trigonometric.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace sr::math;

Transform2D::Transform2D( const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec2& anchor )
: m_Anchor { anchor }
, m_Position { position }
, m_Rotation { rotation }
, m_Scale { scale }
{}

const glm::mat3& Transform2D::getMatrix() const noexcept
{
    if ( m_TransformDirty )
    {
#if 0
        glm::mat3 anchor {
            1, 0, 0,
            0, 1, 0,
            -m_Anchor.x, -m_Anchor.y, 1
        };
        glm::mat3 scale {
            m_Scale.x, 0, 0,
            0, m_Scale.y, 0,
            0, 0, 1
        };
        glm::mat3 rotation {
            glm::cos( m_Rotation ), glm::sin( m_Rotation ), 0,
            -glm::sin( m_Rotation ), glm::cos( m_Rotation ), 0,
            0, 0, 1
        };
        glm::mat3 translation {
            1, 0, 0,
            0, 1, 0,
            m_Position.x, m_Position.y, 1
        };

        m_Transform = translation * rotation * scale * anchor;
#elif 0
        m_Transform = glm::translate( glm::mat3 { 1 }, m_Position );
        m_Transform = glm::rotate( m_Transform, m_Rotation );
        m_Transform = glm::scale( m_Transform, m_Scale );
        m_Transform = glm::translate( m_Transform, -m_Anchor );
#elif 1
        const float s   = glm::sin( m_Rotation );
        const float c   = glm::cos( m_Rotation );
        const float sxc = m_Scale.x * c;
        const float syc = m_Scale.y * c;
        const float sxs = m_Scale.x * s;
        const float sys = m_Scale.y * -s;
        const float tx  = -m_Anchor.x * sxc + -m_Anchor.y * sys + m_Position.x;
        const float ty  = -m_Anchor.x * sxs + -m_Anchor.y * syc + m_Position.y;

        m_Transform = {
            sxc, sxs, 0.0f,
            sys, syc, 0.0f,
            tx, ty, 1.0f
        };
#endif
        m_TransformDirty = false;
    }

    return m_Transform;
}

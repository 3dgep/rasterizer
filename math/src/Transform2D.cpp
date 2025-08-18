#include "glm/ext/matrix_transform.hpp"

#include <Math/Transform2D.hpp>

#include <glm/trigonometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace sr::math;

Transform2D::Transform2D( const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec2& anchor )
: m_Anchor { anchor }
, m_Position { position }
, m_Rotation { rotation }
, m_Scale { scale }
{}

const glm::mat3& Transform2D::getTransform() const noexcept
{
    if ( m_TransformDirty )
    {
         //const float s   = glm::sin( m_Rotation );
         //const float c   = glm::cos( m_Rotation );
         //const float sxc = m_Scale.x * c;
         //const float syc = m_Scale.y * c;
         //const float sxs = m_Scale.x * s;
         //const float sys = m_Scale.y * s;
         //const float tx  = -m_Anchor.x * sxc - m_Anchor.y * sys + m_Position.x;
         //const float ty  = m_Anchor.x * sxs - m_Anchor.y * syc + m_Position.y;

         //m_Transform = {
         //    sxc, -sxs, 0.0f,
         //    sys, syc, 0.0f,
         //    tx, ty, 1.0f
         //};

        //glm::mat3 anchor        = glm::translate( glm::mat3 { 1 }, m_Anchor );
        //glm::mat3 translation   = glm::translate( glm::mat3 { 1 }, m_Position );
        //glm::mat3 rotation      = glm::rotate( glm::mat3 { 1 }, m_Rotation );
        //glm::mat3 scale         = glm::scale( glm::mat3 { 1 }, m_Scale );
        //glm::mat3 inverseAnchor = glm::translate( glm::mat3 { 1 }, -m_Anchor );

        //m_Transform = anchor * translation * rotation * scale * inverseAnchor;

         m_Transform = glm::mat3 { 1 };
         m_Transform = glm::translate( m_Transform, m_Anchor + m_Position );
         m_Transform = glm::rotate( m_Transform, m_Rotation );
         m_Transform = glm::scale( m_Transform, m_Scale );
         m_Transform = glm::translate( m_Transform, -m_Anchor );

        m_TransformDirty = false;
    }

    return m_Transform;
}

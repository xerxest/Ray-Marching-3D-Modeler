//
// References:
// Modeling with distance functions
// http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
//

// primitives

float sdSphere(vec3 _pos, float _radius)
{
	return length(_pos) - _radius;
}

float udBox(vec3 _pos, vec3 _extents)
{
	return length(max(abs(_pos) - _extents, 0.0) );
}

float udRoundBox(vec3 _pos, vec3 _extents, float r)
{
	return length(max(abs(_pos) - _extents, 0.0) ) - r;
}

float sdBox(vec3 _pos, vec3 _extents)
{
	vec3 d = abs(_pos) - _extents;
	return min(max(d.x, max(d.y, d.z) ), 0.0) +
	       length(max(d, 0.0) );
}

float sdTorus(vec3 _pos, vec2 t)
{
	vec2 q = vec2(length(_pos.xz) - t.x, _pos.y);
	return length(q) - t.y;
}

float sdCylinder(vec3 _pos, vec3 c)
{
	return length(_pos.xz - c.xy) - c.z;
}

float sdCone( vec3 p, vec2 c, float h )
{
  vec2 q = h*vec2(c.x/c.y,-1.0);
  vec2 w = vec2( length(p.xz), p.y );
  vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
  vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
  float k = sign( q.y );
  float d = min(dot( a, a ),dot(b, b));
  float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
  return sqrt(d)*sign(s);
}

float sdPlane(vec3 _pos, vec4 n)
{
	// n must be normalized
	return dot(_pos, n.xyz) + n.w;
}

float sdHexPrism(vec3 _pos, vec2 h)
{
	vec3 q = abs(_pos);
	return max(q.z - h.y, max(q.x + q.y * 0.57735, q.y * 1.1547) - h.x);
}

float sdTriPrism(vec3 _pos, vec2 h)
{
	vec3 q = abs(_pos);
	return max(q.z - h.y, max(q.x * 0.866025 + _pos.y * 0.5, -_pos.y) - h.x * 0.5);
}

// domain operations

float opUnion(float d1, float d2)
{
	return min(d1, d2);
}

float opSubtract(float d1, float d2)
{
	return max(-d1, d2);
}

float opIntersect(float d1, float d2)
{
	return max(d1, d2);
}

float opSmoothUnion( float d1, float d2, float k )
{
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

float opSmoothSubtraction( float d1, float d2, float k )
{
    float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
    return mix( d2, -d1, h ) + k*h*(1.0-h);
}

float opSmoothIntersection( float d1, float d2, float k )
{
    float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) + k*h*(1.0-h);
}
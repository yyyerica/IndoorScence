#version 330 core
out vec4 color;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;


uniform vec3 lightPos;
uniform vec3 viewPos;

//uniform float alpha;
void main()
{
	vec3 objectColor = vec3(0.0,0.0,0.0);
	vec3 lightColor = vec3(1.0,1.0,1.0);
//������
	float ambientStrength = 0.3f;
	vec3 ambient = ambientStrength * lightColor;//��������ɫ

//�������
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	//��ķ��������ǹ��λ��������Ƭ�ε�λ������֮���������
	float diff = max(dot(norm, lightDir), 0.0);
	//��norm��lightDir�������е�ˣ��������Ե�ǰƬ�ε�ʵ�ʵ�ɢ��Ӱ��
	//�����������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ����������ᵼ��ɢ�����ӱ�Ϊ������Ϊ�ˣ�����ʹ��max����������������֮��ϴ�Ĳ������Ӷ���֤ɢ�����Ӳ����ɸ���
	vec3 diffuse = diff * lightColor;
	//���ֵ�ٳ��Թ����ɫ���õ�ɢ�����ӣ�ɢ�����ɫ
	
	
	//��Ȼ��������һ������������ɫ��һ��ɢ�����ɫ�����ǰ�������ӣ�Ȼ��ѽ�������������ɫ�������Ƭ�����������ɫ��
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 0.5);
}
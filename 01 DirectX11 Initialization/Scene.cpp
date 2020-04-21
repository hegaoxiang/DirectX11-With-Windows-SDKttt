#include "Scene.h"

#include <fstream>

using namespace std;


namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive& ar, Scene& s, const unsigned int version)
		{
			// serialize base class information
			ar& s.masks;
			ar& s.modelType;
			ar& s.names;
			ar& s.worldMats;
			//ar& s.Num;
		}

	} // namespace serialization
} // namespace boost

void Scene::Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)const
{
	int i = 0;
	for (auto& item : masks)
	{
		if (item & COMPONENT_MODEL)
		{
			DrawItem(i, deviceContext, effect);
		}
		i++;
	}
}

void Scene::Serialize()const
{
	std::ofstream fout("file.txt");// �Ѷ���д��file.txt�ļ���
	boost::archive::text_oarchive oa(fout); // �ı�������鵵�࣬ʹ��һ��ostream������

	oa << *this; // ����obj����
	fout.close();// �ر��ļ�

	return;

}

void Scene::AntiSerialize()
{

	std::ifstream fin("file.txt");

	if (!fin.is_open())
	{
		return;
	}
	boost::archive::text_iarchive ia(fin); // �ı�������鵵��

	ia >> *this; // �ָ���newobj����

	fin.close();

	
	return;
}

void Scene::DrawItem(int i, ID3D11DeviceContext* deviceContext, BasicEffect& effect)const
{
	auto& modelPart = modelParts[i];
	auto& worldMat = worldMats[i];

	UINT strides = modelPart.vertexStride;
	UINT offsets = 0;

	// ���ö���/����������
	deviceContext->IASetVertexBuffers(0, 1, modelPart.vertexBuffer.GetBufferAddress(), &strides, &offsets);
	deviceContext->IASetIndexBuffer(modelPart.indexBuffer.GetBuffer(), modelPart.indexFormat, 0);

	// �������ݲ�Ӧ��
	/*effect.SetWorldMatrix(XMLoadFloat4x4(&worldMat));*/
	effect.SetWorldMatrix(worldMat);

	effect.Apply(deviceContext);

	deviceContext->DrawIndexed(modelPart.indexCount, 0, 0);
}
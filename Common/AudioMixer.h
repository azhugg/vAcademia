
class CAudioMixer
{
public:
	/*
	 * ��������� ��������� �������� ������� � �������� �����.
	 * ��������������, ��� ��� ������ 16������, ����� �����, � ���������� ����������� �������.
	 * bufferSize - ���������� samples * channels
	 */
	static bool MixBuffers(const short** buffers, int bufferCount, int bufferSize, short* outputBuffer);
};

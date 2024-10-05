#include "WebSocketSender.h"

WebSocketSender::WebSocketSender()
{
	//WebSocket ���̺귯�� �����ϱ� 2.2 ����
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	//��� ���� ����, AF_INET = IP v4, SOCK_DGRAM = UDP, 0 = �ڵ� ����
	m_clientSock = socket(AF_INET, SOCK_DGRAM, 0);
	//�ʱ�ȭ
	ZeroMemory(&m_ClientAddr, sizeof(m_ClientAddr));
	m_ClientAddr.sin_family = AF_INET;
	m_ClientAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	//DOMAIN TEST
	/*ADDRINFO hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (!getaddrinfo(SERVER_DOMAIN, PORT, hints, host_domainAddr)) {
		std::cerr << "ERROR\n";
		exit(-1);
		return;
	}

	SOCKADDR_IN* inAddr = (LPSOCKADDR_IN)host_domainAddr->ai_addr;
	m_ClientAddr.sin_addr = inAddr->sin_addr;

	std::cout << inAddr->sin_addr.S_un.S_addr << std::endl;*/
	// TEST END
	m_ClientAddr.sin_port = htons(PORT);

	connected = true;
	set_cameraId();
	std::cout << "UDP WebSocket Connected. \n";
	
}

WebSocketSender::~WebSocketSender()
{
	connected = false;
	//���� �޸� ����
	freeaddrinfo(host_domainAddr);
	WSACleanup();
}

void WebSocketSender::sendframe_via_udp(cv::InputArray frame)
{
	if (!isconnected()) {
		std::cerr << "UDP Not Connected! Failed to send.\n";
		return;
	}
	std::vector<BYTE> bytes = mat2jpg(frame);

	int img_packet_size = bytes.size();
	int total_bytes_sent = 0, sent_bytes, chunk_size;
	BYTE num = 0;
	BYTE buffer[IMG_SEG_SIZE + (sizeof(BYTE) * INFO_SIZE)] = {};

	//�����Ҷ� ������ �� �� 3����Ʈ�� ��Ŷ ������ �Բ� ����
	//�� ����Ʈ�� ������ ��Ŷ����, ī�޶���̵�, �̹��� ��ȣ�� �ǹ��Ѵ�.
	while (total_bytes_sent < img_packet_size) {
		chunk_size = min(IMG_SEG_SIZE, img_packet_size - total_bytes_sent);
		
		memset(buffer, 0, sizeof(buffer));	//0���� �ʱ�ȭ
		buffer[0] = total_bytes_sent + chunk_size < img_packet_size ? 0 : 255;	//������ ��Ŷ���� �˻�
		buffer[1] = camera_id;
		buffer[2] = num++;
		memcpy(buffer + (sizeof(BYTE) * INFO_SIZE), bytes.data() + total_bytes_sent, chunk_size);
		sent_bytes = sendto(m_clientSock, reinterpret_cast<char*>(buffer), chunk_size + (sizeof(BYTE) * INFO_SIZE), 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

		//���� �������� ���� -1�� ���� ������ ���
		if (sent_bytes == SOCKET_ERROR) {
			//check when error occured
			//https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto
			std::cerr << "ERROR code: " << WSAGetLastError() << "\n";
			//delete &bytes;
			return;
		}

		total_bytes_sent += chunk_size;
	}

	std::cout << "Packet sent : " << total_bytes_sent << "\n" << "Seg sent : " << (short)num << "\n";
	//std::cout << "(%) : " << (float)total_bytes_sent / (float)IMG_FULL_SIZE << "\n";
}

std::vector<BYTE> WebSocketSender::mat2jpg(cv::InputArray mat)
{
	std::vector<BYTE> buff;
	cv::imencode(".jpg", mat, buff, encode_param);
	return buff;
}

void WebSocketSender::set_cameraId()
{
	int id;
	std::cout << "Please, Enter Camera ID : ";
	std::cin >> id;
	camera_id = (BYTE)id;
}

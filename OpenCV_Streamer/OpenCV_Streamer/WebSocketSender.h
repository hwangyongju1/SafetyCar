#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

//upd max packet size	65535 bytes
//mat 2 jpg size		about 23805 ?
//cv::Mat size			mat.step[0] * mat.rows;
//						mat.total() * mat,elemSize();
//						640 * 640 * 3 = 1,228,800 bytes
//						1,228,800 / 20 = 61,440

//MTU�� ���� 1500���� �����Ǿ� �ִ�.
//CMD���� �̸� ������ �� ������ ������� �ʴ´ٰ� �Ѵ�.
//���� ��Ŷ�� 1500�� ���߾� �߶� ������ ������ �ʿ��ϴ�.
#define PORT			5432
#define MTU				1500
#define UDP_HEADER_SIZE 28
#define INFO_SIZE		3						//ī�޶� ��ȣ �� �̹��� ��ȣ�� ��� ������ ����� ���� ��Ŷ ���(UDP ����� ����)�� ũ��(����Ʈ) - End_Flag, CameraID, ImageSegNum
#define PACKET_SIZE		MTU - UDP_HEADER_SIZE	//MTU - UPD-Header = 1500 - 28 = 1472
#define IMG_SEG_SIZE	PACKET_SIZE - INFO_SIZE
#define IMG_QUALITY		95						//jpeg �������� ��ȯ�Ҷ� ȭ�� ����
//#define SERVER_IP		"127.0.0.1"
#define SERVER_IP		"43.202.61.242"
#define SERVER_DOMAIN	"j11b209.p.ssafy.io"

//�̹��� Mat�� ũ��
constexpr int IMG_FULL_SIZE = 640 * 480 * 3;

class WebSocketSender
{
public:
	WebSocketSender();
	~WebSocketSender();

	//������ ����
	void sendframe_via_udp(cv::InputArray frame);

	std::vector<BYTE> mat2jpg(cv::InputArray mat);
	inline bool isconnected() { return connected; }
private:
	bool connected;
	BYTE camera_id;
	WSADATA wsadata;
	SOCKET m_clientSock;
	SOCKADDR_IN m_ClientAddr;
	ADDRINFO* host_domainAddr;
	std::vector<int> encode_param = { cv::IMWRITE_JPEG_QUALITY, IMG_QUALITY };
	void set_cameraId();	
};


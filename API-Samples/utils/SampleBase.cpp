#include <iostream>
#include <chrono>

#include "SampleBase.hpp"

void SampleBase::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(info.window, NULL);
		break;
	case WM_LBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButtons.left = true;
		break;
	case WM_RBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButtons.right = true;
		break;
	case WM_MBUTTONDOWN:
		mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		mouseButtons.middle = true;
		break;
	case WM_LBUTTONUP:
		mouseButtons.left = false;
		break;
	case WM_RBUTTONUP:
		mouseButtons.right = false;
		break;
	case WM_MBUTTONUP:
		mouseButtons.middle = false;
		break;
	case WM_MOUSEWHEEL:
	{
		short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		camera.zoom += (float)wheelDelta * 0.005f * camera.zoomSpeed;
		camera.setZoom(camera.zoom);
	}
		break;
	case WM_MOUSEMOVE:
		handleMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		break;
	}
}

void SampleBase::handleMouseMove(int32_t x, int32_t y)
{
	int32_t dx = (int32_t)mousePos.x - x;
	int32_t dy = (int32_t)mousePos.y - y;

	if (mouseButtons.left) {
		glm::mat4 rotM = glm::mat4(1.0f);
		rotM = glm::rotate(rotM, glm::radians(dy * camera.rotationSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(dx * camera.rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

		camera.setPosition((glm::vec3)(rotM * glm::vec4(camera.position, 1)));
	}
	if (mouseButtons.right) {
		camera.zoom += dy * .005f * camera.zoomSpeed;

		camera.setZoom(camera.zoom);
	}
	if (mouseButtons.middle) {
		camera.lookat.x -= dx * 0.01f;
		camera.lookat.y -= dy * 0.01f;

		camera.setLookat(camera.lookat);
	}

	mousePos = glm::vec2((float)x, (float)y);
}

void SampleBase::renderLoop()
{
#if defined(_WIN32)
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!IsIconic(info.window)) {
			renderFrame();
		}
	}
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
#elif defined(_DIRECT2DISPLAY)
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#elif defined(VK_USE_PLATFORM_XCB_KHR)
#endif

	// Flush device to make sure all resources can be freed
	if (info.device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(info.device);
	}
}

auto _lastTime = std::chrono::high_resolution_clock::now();
void SampleBase::renderFrame()
{
	auto time = std::chrono::high_resolution_clock::now();
	auto tDiff = std::chrono::duration<double, std::milli>(time - _lastTime).count();
	float deltaTime = (float)tDiff / 1000.0f;
	_lastTime = time;

//	std::cout << deltaTime << std::endl;

	update(deltaTime);

	render();


}
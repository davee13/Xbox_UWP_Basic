#pragma once

#include "pch.h"
#include "Common\DeviceResources.h"
#include "App1Main.h"


namespace App1
{
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		App();

		
		// IFrameworkView Methods.
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers.
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		//Mouse
		void OnMouseMoved(_In_ Windows::Devices::Input::MouseDevice^ mouseDevice, _In_ Windows::Devices::Input::MouseEventArgs^ args);
		//void OnKeyPressed(_In_ Windows::Devices::Input::KeyboardCapabilities^ keyboardDevice, _In_ Windows::Devices::Input::k^ args);

		//void Grid_KeyUp(_In_ Platform::Object^ sender, Windows::UI::Xaml::Input::KeyboardAcceleratorInvokedEventArgs^ args);

		void BackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ e);


		void OnKeyDown(_In_ Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
		void OnKeyUp(_In_ Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);

		//void CoreWindow_KeyDown(_In_ Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
		void OnRawControllerAdded(Platform::Object^ sender, RawGameController^ args);
		void OnRawControllerRemoved(Platform::Object^ sender, RawGameController^ args);


	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<App1Main> m_main;
		//std::shared_ptr<CoreWindow> &iWindow;
		bool m_windowClosed;
		bool m_windowVisible;
		//float m_pitch;                     // mouse y increases down, but pitch increases up
		//float m_yaw;
		//std::unique_ptr<DirectX::Keyboard> m_keyboard;



	};
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};

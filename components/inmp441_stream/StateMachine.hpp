#include <tinyfsm.hpp>

namespace esphome {
namespace inmp441_stream {

struct WifiConnected;
struct WifiDisconnected;
struct MQTTConnected;
struct MQTTDisconnected;
struct Listening;
struct ListeningPlay;
struct Idle;
struct IdlePlay;
struct Tts;
struct TtsPlay;
struct Error;
struct ErrorPlay;
struct Updating;

struct WifiDisconnectEvent : tinyfsm::Event {};
struct WifiConnectEvent : tinyfsm::Event {};
struct MQTTDisconnectedEvent : tinyfsm::Event {};
struct MQTTConnectedEvent : tinyfsm::Event {};
struct IdleEvent : tinyfsm::Event {};
struct TtsEvent : tinyfsm::Event {};
struct ErrorEvent : tinyfsm::Event {};
struct UpdateEvent : tinyfsm::Event {};
struct BeginPlayAudioEvent : tinyfsm::Event {};
struct EndPlayAudioEvent : tinyfsm::Event {};
struct StreamAudioEvent : tinyfsm::Event {};
struct PlayBytesEvent : tinyfsm::Event {};
struct ListeningEvent : tinyfsm::Event {};
struct UpdateConfigurationEvent : tinyfsm::Event {};

class StateMachine : public tinyfsm::Fsm<StateMachine> {
  virtual void run(void){};
  void exit(void){};
  bool hotwordDetected = false;
};

using fsm = tinyfsm::Fsm<StateMachine>;

}  // namespace inmp441_stream
}  // namespace esphome

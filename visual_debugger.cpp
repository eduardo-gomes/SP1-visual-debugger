#include <DuEngine/DuEngine.hpp>

#include "emulador.hpp"

namespace scene{
class Visual_debugger: public BaseScene {
   private:

   public:
	virtual void Update(int64_t);
	virtual void Render() {}
	virtual void RenderGUI();
};
}
void Setup(){
	new scene::Visual_debugger;
	cpu::reset();
	cpu::load_program("a.sp1");
}
int main(){
	SetSetup(Setup);
	Start("Visual_debugger");
}
void scene::Visual_debugger::Update(int64_t) {
	//cpu::step();
}

void scene::Visual_debugger::RenderGUI() {
	ImGui::Begin("Registers");
	if(ImGui::Button("Step")) cpu::step();
	ImGui::Text("ACC %hu", cpu::acc);

	ImGui::Text("PC: %hu (%s)", cpu::pc, cpu::disassString(cpu::pc).c_str());

	ImGui::Text("Disassembler");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 25; n++) {
		ImGui::Text("%03d: %s", n*2, cpu::disassString(n*2).c_str());
	}
	ImGui::EndChild();
	ImGui::End();
	ImGui::Begin("Memory");
	for (int n = 0; n < 0xFF; n++) 
		ImGui::Text("%02d: %hu", n, cpu::memory[n]);
	ImGui::End();
	}
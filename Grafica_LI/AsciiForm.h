#pragma once

namespace Grafica_LI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for AsciiForm
	/// </summary>
	public ref class AsciiForm : public System::Windows::Forms::Form
	{
	public:
		AsciiForm(System::String^ str, Color color)
		{
			InitializeComponent();
			textBox1->Text = str;
			textBox1->BackColor = Color::Black;
			textBox1->ForeColor = color;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AsciiForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox1;
	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Consolas", 1, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Pixel,
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(0, 0);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(513, 435);
			this->textBox1->TabIndex = 0;
			this->textBox1->WordWrap = false;
			// 
			// AsciiForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(513, 435);
			this->Controls->Add(this->textBox1);
			this->Name = L"AsciiForm";
			this->Text = L"Reprezentare ASCII";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}

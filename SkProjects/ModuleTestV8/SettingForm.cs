using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace ModuleTestV8
{
    public partial class SettingForm : Form
    {
        //[DllImport("kernel32", CharSet = CharSet.Unicode)]
        //private static extern long WritePrivateProfileString(string section,
        //    string key, string val, string filePath);

        //[DllImport("kernel32", CharSet = CharSet.Unicode)]
        //private static extern int GetPrivateProfileString(string section,
        //    string key, string def, StringBuilder retVal, int size, string filePath);

        private ModuleTestProfile profile;

        public SettingForm()
        {
            InitializeComponent();
            profile = new ModuleTestProfile();

        }

        public SettingForm(ModuleTestProfile p)
        {
            InitializeComponent();
            profile = p;

        }

        private void OK_Click(object sender, EventArgs e)
        {
            profile.gdBaudSel = gdBaudSel.SelectedIndex;
            DialogResult = DialogResult.OK;
            Close();
        }

        private void InjectionPassCriteria(ComboBox c)
        {
            c.Items.AddRange(ModuleTestProfile.CriteriaStrings);
        }

        private void InjectionModule(ComboBox c, List<String> s)
        {
            foreach (String m in s)
            {
                c.Items.Add(m);
            }
        }
        private void SettingForm_Load(object sender, EventArgs e)
        {

            Global.InjectionBaudRate(gdBaudSel);
            //InjectionBaudRate(dvBaudSel);
            Global.InjectionBaudRate(dlBaudSel);

            //InjectionPassCriteria(gpPassSel);
            //InjectionPassCriteria(glPassSel);
            //InjectionPassCriteria(bdPassSel);
            //InjectionPassCriteria(gaPassSel);

            String iniFile = Environment.CurrentDirectory + "\\Module.ini";
            List<String> rGps = new List<String>();
            List<String> rGlonass = new List<String>();
            List<String> rBeidou = new List<String>();
            List<String> rGalileo = new List<String>();

            ModuleIniParser.ErrorCode er = ModuleIniParser.Load(iniFile, ref rGps, ref rGlonass, ref rBeidou, ref rGalileo);
            if(er==ModuleIniParser.ErrorCode.NoGpsModule)
            {
                ErrorMessage.Show(ErrorMessage.Errors.NoGpsModule);
                DialogResult = DialogResult.Cancel;
                Close();
                return;
            }
            InjectionModule(gpModuleSel, rGps);
            InjectionModule(glModuleSel, rGlonass);
            InjectionModule(bdModuleSel, rBeidou);
            InjectionModule(gaModuleSel, rGalileo);

            AdjustUIByProfile();
        }

        public enum ModuleTypes { GpsModule, GlonassModule, BeidouModule, GalileoModule }
        private void AdjustUIByProfile()
        {
            gdBaudSel.SelectedIndex = profile.gdBaudSel;

            gpModuleSel.SelectedIndex = profile.gpModuleSel;
            if (glModuleSel.Items.Count > 0)
            {
                glModuleSel.SelectedIndex = profile.glModuleSel;
            }
            else
            {
                glModule.Enabled = false;
            }

            if (bdModuleSel.Items.Count > 0)
            {
                bdModuleSel.SelectedIndex = profile.bdModuleSel;
            }
            else
            {
                bdModule.Enabled = false;
            }

            if (gaModuleSel.Items.Count > 0)
            {
                gaModuleSel.SelectedIndex = profile.gaModuleSel;
            }
            else
            {
                gaModule.Enabled = false;
            }

            switch ((ModuleTypes)profile.moduleType)
            {
                case ModuleTypes.GpsModule:
                    gpModule.Checked = true;
                    break;
                case ModuleTypes.GlonassModule:
                    glModule.Checked = true;
                    break;
                case ModuleTypes.BeidouModule:
                    bdModule.Checked = true;
                    break;
                case ModuleTypes.GalileoModule:
                    gaModule.Checked = true;
                    break;
            }
            //dvBaudSel.SelectedIndex = profile.dvBaudRateIdx;

            gpPassCriteria.Checked = profile.testGpSnr;
            glPassCriteria.Checked = profile.testGlSnr;
            bdPassCriteria.Checked = profile.testBdSnr;
            gaPassCriteria.Checked = profile.testGaSnr;

            //gpPassSel.SelectedIndex = profile.gpPassSel;
            //glPassSel.SelectedIndex = profile.glPassSel;
            //bdPassSel.SelectedIndex = profile.bdPassSel;
            //gaPassSel.SelectedIndex = profile.gaPassSel;
            gpSnrUpper.Text = profile.gpSnrUpper.ToString();
            gpSnrLower.Text = profile.gpSnrLower.ToString();
            glSnrUpper.Text = profile.glSnrUpper.ToString();
            glSnrLower.Text = profile.glSnrLower.ToString();
            bdSnrUpper.Text = profile.bdSnrUpper.ToString();
            bdSnrLower.Text = profile.bdSnrLower.ToString();
            gaSnrUpper.Text = profile.gaSnrUpper.ToString();
            gaSnrLower.Text = profile.gaSnrLower.ToString();
            
            snrTestPeriod.Text = profile.snrTestPeriod.ToString();

            gpSnrLimit.Text = profile.gpSnrLimit.ToString();
            glSnrLimit.Text = profile.glSnrLimit.ToString();
            bdSnrLimit.Text = profile.bdSnrLimit.ToString();
            gaSnrLimit.Text = profile.gaSnrLimit.ToString();

            writeTag.Checked = profile.writeTag;
            iniFileName.Text = profile.iniFileName;
            enableDownload.Checked = profile.enableDownload;
            dlBaudSel.SelectedIndex = profile.dlBaudSel;
            dlBaudSel.Enabled = enableDownload.Checked;

            //promFileName.Text = profile.promFileName;
            //testBootStatus.Checked = profile.testBootStatus;
            //testPromCrc.Checked = profile.testPromCrc;
            //promCrc.Text = profile.promCrc.ToString("X");
            checkPromCrc.Checked = profile.checkPromCrc;
            testClockOffset.Checked = profile.testClockOffset;
            clockOffsetThreshold.Text = profile.clockOffsetThreshold.ToString();
            writeClockOffset.Checked = profile.writeClockOffset;
            testEcompass.Checked = profile.testEcompass;
            testMiniHommer.Checked = profile.testMiniHommer;
            testDrCyro.Checked = profile.testDrCyro;
            testDrDuration.Text = profile.testDrDuration.ToString();
            uslClockWise.Text = profile.uslClockWise.ToString();
            uslAnticlockWise.Text = profile.uslAnticlockWise.ToString();
            lslClockWise.Text = profile.lslClockWise.ToString();
            lslAnticlockWise.Text = profile.lslAnticlockWise.ToString();
            thresholdCogWise.Text = profile.thresholdCog.ToString();


        }
        private void gpModule_CheckedChanged(object sender, EventArgs e)
        {
            profile.moduleType = (int)ModuleTypes.GpsModule;
            if (((RadioButton)sender).Checked)
            {
                gpModuleSel.Enabled = true;
                glModuleSel.Enabled = false;
                bdModuleSel.Enabled = false;
                gaModuleSel.Enabled = false;
                moduleName.Text = gpModuleSel.SelectedItem.ToString();
            }
        }

        private void glModule_CheckedChanged(object sender, EventArgs e)
        {
            profile.moduleType = (int)ModuleTypes.GlonassModule;
            if (((RadioButton)sender).Checked)
            {
                gpModuleSel.Enabled = false;
                glModuleSel.Enabled = true;
                bdModuleSel.Enabled = false;
                gaModuleSel.Enabled = false;
                moduleName.Text = glModuleSel.SelectedItem.ToString();
            }
        }

        private void bdModule_CheckedChanged(object sender, EventArgs e)
        {
            profile.moduleType = (int)ModuleTypes.BeidouModule;
            if (((RadioButton)sender).Checked)
            {
                gpModuleSel.Enabled = false;
                glModuleSel.Enabled = false;
                bdModuleSel.Enabled = true;
                gaModuleSel.Enabled = false;
                moduleName.Text = bdModuleSel.SelectedItem.ToString();
            }
        }

        private void gaModule_CheckedChanged(object sender, EventArgs e)
        {
            profile.moduleType = (int)ModuleTypes.GalileoModule;
            if (((RadioButton)sender).Checked)
            {
                gpModuleSel.Enabled = false;
                glModuleSel.Enabled = false;
                bdModuleSel.Enabled = false;
                gaModuleSel.Enabled = true;
                moduleName.Text = gaModuleSel.SelectedItem.ToString();
            }
        }

        private void gpModuleSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (((ComboBox)sender).SelectedIndex == profile.gpModuleSel)
            {
                return;
            }
            profile.gpModuleSel = ((ComboBox)sender).SelectedIndex;
            moduleName.Text = gpModuleSel.SelectedItem.ToString();
        }

        private void glModuleSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (((ComboBox)sender).SelectedIndex == profile.glModuleSel)
            {
                return;
            }
            profile.glModuleSel = ((ComboBox)sender).SelectedIndex;
            moduleName.Text = glModuleSel.SelectedItem.ToString();
        }

        private void bdModuleSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (((ComboBox)sender).SelectedIndex == profile.bdModuleSel)
            {
                return;
            }
            profile.bdModuleSel = ((ComboBox)sender).SelectedIndex;
            moduleName.Text = bdModuleSel.SelectedItem.ToString();
        }

        private void gaModuleSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (((ComboBox)sender).SelectedIndex == profile.gaModuleSel)
            {
                return;
            }
            profile.gaModuleSel = ((ComboBox)sender).SelectedIndex;
            moduleName.Text = gaModuleSel.SelectedItem.ToString();
        }

        private void moduleName_TextChanged(object sender, EventArgs e)
        {
            profile.moduleName = moduleName.Text;
        }

        private void gdBaudSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            profile.gdBaudSel = (sender as ComboBox).SelectedIndex;
        }

        private void dlBaudSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            profile.dlBaudSel = (sender as ComboBox).SelectedIndex;
        }

        //private void dvBaudSel_SelectedIndexChanged(object sender, EventArgs e)
        //{
        //    profile.dvBaudRateIdx = ((ComboBox)sender).SelectedIndex;
        //}

        //private void gpPassSel_SelectedIndexChanged(object sender, EventArgs e)
        //{
        //    profile.gpPassSel = (sender as ComboBox).SelectedIndex;
        //}

        //private void glPassSel_SelectedIndexChanged(object sender, EventArgs e)
        //{
        //    profile.glPassSel = (sender as ComboBox).SelectedIndex;
        //}

        //private void bdPassSel_SelectedIndexChanged(object sender, EventArgs e)
        //{
        //    profile.bdPassSel = (sender as ComboBox).SelectedIndex;
        //}

        //private void gaPassSel_SelectedIndexChanged(object sender, EventArgs e)
        //{
        //    profile.gaPassSel = (sender as ComboBox).SelectedIndex;
        //}

        private void gpPassCriteria_CheckedChanged(object sender, EventArgs e)
        {
            profile.testGpSnr = (sender as CheckBox).Checked;
            //gpPassSel.Enabled = (sender as CheckBox).Checked;
            gpSnrUpper.Enabled = (sender as CheckBox).Checked;
            gpSnrLower.Enabled = (sender as CheckBox).Checked;
            gpSnrLimit.Enabled = (sender as CheckBox).Checked;
        }

        private void glPassCriteria_CheckedChanged(object sender, EventArgs e)
        {
            profile.testGlSnr = (sender as CheckBox).Checked;
            //glPassSel.Enabled = (sender as CheckBox).Checked;
            glSnrUpper.Enabled = (sender as CheckBox).Checked;
            glSnrLower.Enabled = (sender as CheckBox).Checked;
            glSnrLimit.Enabled = (sender as CheckBox).Checked;
        }

        private void bdPassCriteria_CheckedChanged(object sender, EventArgs e)
        {
            profile.testBdSnr = (sender as CheckBox).Checked;
            //bdPassSel.Enabled = (sender as CheckBox).Checked;
            bdSnrUpper.Enabled = (sender as CheckBox).Checked;
            bdSnrLower.Enabled = (sender as CheckBox).Checked;
            bdSnrLimit.Enabled = (sender as CheckBox).Checked;
        }

        private void gaPassCriteria_CheckedChanged(object sender, EventArgs e)
        {
            profile.testGaSnr = (sender as CheckBox).Checked;
            //gaPassSel.Enabled = (sender as CheckBox).Checked;
            gaSnrUpper.Enabled = (sender as CheckBox).Checked;
            gaSnrLower.Enabled = (sender as CheckBox).Checked;
            gaSnrLimit.Enabled = (sender as CheckBox).Checked;
        }

        private void snrTestPeriod_TextChanged(object sender, EventArgs e)
        {
            profile.snrTestPeriod = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private void gpSnrLimit_TextChanged(object sender, EventArgs e)
        {
            profile.gpSnrLimit = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private void glSnrLimit_TextChanged(object sender, EventArgs e)
        {
            profile.glSnrLimit = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private void bdSnrLimit_TextChanged(object sender, EventArgs e)
        {
            profile.bdSnrLimit = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private void gaSnrLimit_TextChanged(object sender, EventArgs e)
        {
            profile.gaSnrLimit = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private int GetTextBoxSnrInt(TextBox t)
        {
            int value = 0;
            try
            {
                value = Convert.ToInt32(t.Text);
                t.ForeColor = (value >= -999 || value <= 999) ? Color.Black : Color.Red;
            }
            catch
            {
                t.ForeColor = Color.Red;
            }
            return value;
        }

        private void writeTag_CheckedChanged(object sender, EventArgs e)
        {
            profile.writeTag = (sender as CheckBox).Checked;
            //tagIniFile.Enabled = ((CheckBox)sender).Checked;
            //browseIni.Enabled = ((CheckBox)sender).Checked;
        }

        private void iniFileName_TextChanged(object sender, EventArgs e)
        {
            profile.iniFileName = (sender as TextBox).Text;
        }

        private void browseIni_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();
            openFileDlg.InitialDirectory = Login.loginInfo.currentPath;
            openFileDlg.Filter = "ini files (*.ini)|*.ini|All files (*.*)|*.*";
            openFileDlg.FilterIndex = 1;
            openFileDlg.RestoreDirectory = true;

            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                //It will crash in Test PC (3F).
                //iniFileName.Text = openFileDlg.SafeFileName;
                iniFileName.Text = Path.GetFileName(openFileDlg.FileName);
            }
        }

        private void enableDownload_CheckedChanged(object sender, EventArgs e)
        {
            profile.enableDownload = (sender as CheckBox).Checked;
            dlBaudSel.Enabled = (sender as CheckBox).Checked;
            //promFileName.Enabled = (sender as CheckBox).Checked;
            //browseFw.Enabled = (sender as CheckBox).Checked;
        }    

        private void browseFw_Click(object sender, EventArgs e)
        {
            /*
            OpenFileDialog openFileDlg = new OpenFileDialog();

            openFileDlg.InitialDirectory = Login.loginInfo.currentPath;
            openFileDlg.Filter = "bin files (*.bin)|*.bin|All files (*.*)|*.*";
            openFileDlg.FilterIndex = 1;
            openFileDlg.RestoreDirectory = true;

            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                promFileName.Text = openFileDlg.SafeFileName;
            }
            */
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void testBootStatus_CheckedChanged(object sender, EventArgs e)
        {
            //profile.testBootStatus = (sender as CheckBox).Checked;
        }

        private void testPromCrc_CheckedChanged(object sender, EventArgs e)
        {
            //profile.checkPromCrc = (sender as CheckBox).Checked;
        }

        private void testClockOffset_CheckedChanged(object sender, EventArgs e)
        {
            profile.testClockOffset = (sender as CheckBox).Checked;
            clockOffsetThreshold.Enabled = (sender as CheckBox).Checked;
            writeClockOffset.Enabled = (sender as CheckBox).Checked;
        }

        private double GetTextBoxPositiveDouble(TextBox t)
        {
            double value = 0.0;
            try
            {
                value = Convert.ToDouble(t.Text);
                t.ForeColor = (value >= 0.0) ? Color.Black : Color.Red;
            }
            catch
            {
                t.ForeColor = Color.Red;
            }
            return value;
        }

        private void clockOffsetThreshold_TextChanged(object sender, EventArgs e)
        {
            profile.clockOffsetThreshold = GetTextBoxPositiveDouble((TextBox)sender);
        }

        private void writeClockOffset_CheckedChanged(object sender, EventArgs e)
        {
            profile.writeClockOffset = (sender as CheckBox).Checked;
       }

        private void testEcompass_CheckedChanged(object sender, EventArgs e)
        {
            profile.testEcompass = (sender as CheckBox).Checked;
        }

        private void testMiniHommer_CheckedChanged(object sender, EventArgs e)
        {
            profile.testMiniHommer = (sender as CheckBox).Checked;
        }

        private void testDrCyro_CheckedChanged(object sender, EventArgs e)
        {
            profile.testDrCyro = (sender as CheckBox).Checked;
            testDrDuration.Enabled = (sender as CheckBox).Checked;
            uslClockWise.Enabled = (sender as CheckBox).Checked;
            uslAnticlockWise.Enabled = (sender as CheckBox).Checked;
            lslClockWise.Enabled = (sender as CheckBox).Checked;
            lslAnticlockWise.Enabled = (sender as CheckBox).Checked;
            thresholdCogWise.Enabled = (sender as CheckBox).Checked;
        }

        private void testDrDuration_TextChanged(object sender, EventArgs e)
        {
            profile.testDrDuration = Global.GetTextBoxPositiveInt(sender as TextBox);
        }

        private void uslClockWise_TextChanged(object sender, EventArgs e)
        {
            profile.uslClockWise = GetTextBoxPositiveDouble(sender as TextBox);
        }

        private void uslAnticlockWise_TextChanged(object sender, EventArgs e)
        {
            profile.uslAnticlockWise = GetTextBoxPositiveDouble(sender as TextBox);
        }

        private void lslClockWise_TextChanged(object sender, EventArgs e)
        {
            profile.lslClockWise = GetTextBoxPositiveDouble(sender as TextBox);
        }

        private void lslAnticlockWise_TextChanged(object sender, EventArgs e)
        {
            profile.lslAnticlockWise = GetTextBoxPositiveDouble(sender as TextBox);
        }

        private void thresholdCogWise_TextChanged(object sender, EventArgs e)
        {
            profile.thresholdCog = GetTextBoxPositiveDouble(sender as TextBox);
        }

        private void promFileName_TextChanged(object sender, EventArgs e)
        {

        }

        private void saveAs_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDlg = new SaveFileDialog();

            saveFileDlg.InitialDirectory = Login.loginInfo.currentPath;
            saveFileDlg.Filter = "dat files (*.dat)|*.dat|All files (*.*)|*.*";
            saveFileDlg.FilterIndex = 1;
            saveFileDlg.RestoreDirectory = false;
            saveFileDlg.FileName = Form1.DefaultProfileName;

            if (saveFileDlg.ShowDialog() == DialogResult.OK)
            {
                profile.SaveToIniFile(saveFileDlg.FileName);
            }
        }

        private bool CheckProfileValidity(ModuleTestProfile p)
        {
            if (p.gpModuleSel > gpModuleSel.Items.Count)
            {
                ErrorMessage.Show(ErrorMessage.Errors.InvalidGpsModule);
                return false;
            }
            if (p.glModuleSel > glModuleSel.Items.Count)
            {
                ErrorMessage.Show(ErrorMessage.Errors.InvalidGlonassModule);
                return false;
            }
            if (p.bdModuleSel > bdModuleSel.Items.Count)
            {
                ErrorMessage.Show(ErrorMessage.Errors.InvalidBeidouModule);
                return false;
            }
            if (p.gaModuleSel > gaModuleSel.Items.Count)
            {
                ErrorMessage.Show(ErrorMessage.Errors.InvalidGalileoModule);
                return false;
            }
            if ((ModuleTypes.GpsModule == (ModuleTypes)p.moduleType && 
                    0 == gpModuleSel.Items.Count) || 
                (ModuleTypes.GlonassModule == (ModuleTypes)p.moduleType && 
                    0 == glModuleSel.Items.Count) ||                
                (ModuleTypes.BeidouModule == (ModuleTypes)p.moduleType && 
                    0 == bdModuleSel.Items.Count) ||                
                (ModuleTypes.GalileoModule == (ModuleTypes)p.moduleType && 
                    0 == gaModuleSel.Items.Count) )             
            {
                ErrorMessage.Show(ErrorMessage.Errors.ProfileHasInvalidModule);
                return false;
            }
            return true;
        }

        private static bool CheckProfileValidityInModule(ModuleTestProfile p, bool silent)
        {
            String iniFile = Environment.CurrentDirectory + "\\Module.ini";
            List<String> rGps = new List<String>();
            List<String> rGlonass = new List<String>();
            List<String> rBeidou = new List<String>();
            List<String> rGalileo = new List<String>();

            ModuleIniParser.ErrorCode er = ModuleIniParser.Load(iniFile, ref rGps, ref rGlonass, ref rBeidou, ref rGalileo);
            if (er == ModuleIniParser.ErrorCode.NoGpsModule)
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.NoGpsModule);
                    MessageBox.Show(iniFile);
                }
                return false;
            }
            if (p.gpModuleSel > rGps.Count)
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.InvalidGpsModule);
                }
                return false;
            }
            if (p.glModuleSel > rGlonass.Count)
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.InvalidGlonassModule);
                }
                return false;
            }
            if (p.bdModuleSel > rBeidou.Count)
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.InvalidBeidouModule);
                }
                return false;
            }
            if (p.gaModuleSel > rGalileo.Count)
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.InvalidGalileoModule);
                }
                return false;
            }
            if ((ModuleTypes.GpsModule == (ModuleTypes)p.moduleType &&
                    0 == rGps.Count) ||
                (ModuleTypes.GlonassModule == (ModuleTypes)p.moduleType &&
                    0 == rGlonass.Count) ||
                (ModuleTypes.BeidouModule == (ModuleTypes)p.moduleType &&
                    0 == rBeidou.Count) ||
                (ModuleTypes.GalileoModule == (ModuleTypes)p.moduleType &&
                    0 == rGalileo.Count))
            {
                if (!silent)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.ProfileHasInvalidModule);
                }
                return false;
            }
            return true;
        }

        public static ModuleTestProfile LoadAndCheckProfile(String path, bool silent)
        {
            ModuleTestProfile p = new ModuleTestProfile();
            if (!p.LoadFromIniFile(path))
            {
                if (!silent && p.error == ModuleTestProfile.ErrorCode.InvalidateFormat)
                {
                    ErrorMessage.Show(ErrorMessage.Errors.ProfileFormatError);
                }
                return null;
            }
            if (!CheckProfileValidityInModule(p, silent))
            {
                return null;
            }
            return p;
        }
        private void loadFrom_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();

            openFileDlg.InitialDirectory = Login.loginInfo.currentPath;
            openFileDlg.Filter = "dat files (*.dat)|*.dat|All files (*.*)|*.*";
            openFileDlg.FilterIndex = 1;
            openFileDlg.RestoreDirectory = true;

            if (openFileDlg.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            ModuleTestProfile p = LoadAndCheckProfile(openFileDlg.FileName, false);
            if (p == null)
            {
                return;
            }
            AdjustUIByProfile();
        }

        //private void gpSnrUppler_TextChanged(object sender, EventArgs e)
        //{
        //    profile.gpSnrLimit = GetTextBoxPositiveInt(sender as TextBox);
        //}

        private void gpSnrLower_TextChanged(object sender, EventArgs e)
        {
            profile.gpSnrLower = GetTextBoxSnrInt(sender as TextBox);
        }

        private void glSnrLower_TextChanged(object sender, EventArgs e)
        {
            profile.glSnrLower = GetTextBoxSnrInt(sender as TextBox);
        }

        private void bdSnrLower_TextChanged(object sender, EventArgs e)
        {
            profile.bdSnrLower = GetTextBoxSnrInt(sender as TextBox);
        }

        private void gaSnrLower_TextChanged(object sender, EventArgs e)
        {
            profile.gaSnrLower = GetTextBoxSnrInt(sender as TextBox);
        }

        private void gpSnrUpper_TextChanged(object sender, EventArgs e)
        {
            profile.gpSnrUpper = GetTextBoxSnrInt(sender as TextBox);
        }

        private void glSnrUpper_TextChanged(object sender, EventArgs e)
        {
            profile.glSnrUpper = GetTextBoxSnrInt(sender as TextBox);
        }

        private void bdSnrUpper_TextChanged(object sender, EventArgs e)
        {
            profile.bdSnrUpper = GetTextBoxSnrInt(sender as TextBox);
        }

        private void gaSnrUpper_TextChanged(object sender, EventArgs e)
        {
            profile.gaSnrUpper = GetTextBoxSnrInt(sender as TextBox);
        }

        private void checkPromCrc_CheckedChanged(object sender, EventArgs e)
        {
            profile.checkPromCrc = (sender as CheckBox).Checked;
        }


    }
}

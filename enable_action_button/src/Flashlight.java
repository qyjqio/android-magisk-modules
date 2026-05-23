import android.hardware.camera2.CameraManager;
import android.content.Context;
import android.os.Looper;
import java.io.*;

public class Flashlight {
    private static final String STATE_FILE = "/data/local/tmp/.torch_state";

    public static void main(String[] args) {
        Looper.prepareMainLooper();
        try {
            Class<?> atCls = Class.forName("android.app.ActivityThread");
            Object at = atCls.getMethod("systemMain").invoke(null);
            Context ctx = (Context) atCls.getMethod("getSystemContext").invoke(at);
            CameraManager cm = (CameraManager) ctx.getSystemService("camera");

            boolean on;
            if (args.length > 0 && args[0].equals("toggle")) {
                on = !readState();
            } else {
                on = args.length > 0 && (args[0].equals("1") || args[0].equals("on"));
            }

            for (int id = 0; id <= 2; id++) {
                try {
                    cm.setTorchMode(String.valueOf(id), on);
                    writeState(on);
                    System.out.println("torch " + (on ? "on" : "off"));
                    System.exit(0);
                } catch (Exception e) {}
            }
            System.out.println("torch failed");
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    private static boolean readState() {
        try {
            BufferedReader r = new BufferedReader(new FileReader(STATE_FILE));
            String s = r.readLine();
            r.close();
            return "1".equals(s);
        } catch (Exception e) {
            return false;
        }
    }

    private static void writeState(boolean on) {
        try {
            PrintWriter w = new PrintWriter(new FileWriter(STATE_FILE));
            w.println(on ? "1" : "0");
            w.close();
        } catch (Exception e) {}
    }
}
